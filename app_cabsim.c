#include "flexfx.h" // Defines config variables, I2C and GPIO functions, DSP functions, etc.
#include <math.h>   // Floating point for filter coeff calculations in the background process.
#include <string.h>

// System configuration ...

const char* company_name_string    = "BitStream"; // Your company name
const char* product_name_string    = "FlexFX";    // Your product name
const int   default_sample_rate    = 48000;       // Default sample rate at boot-up
const int   supported_sample_rates = 0x02;        // 1=44k1, 2=48k0, 4=88k2, ... 32=192k0
const int   dsp_channel_count      = 32;          // 32 channels to/from each DSP thread
const int   usb_output_chan_count  = 2;           // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;           // 2 USB audio class 2.0 input channels
const int   i2s_tdm_slot_count     = 2;           // 2 for I2S (Stereo), 4 for I4S, 8 = I8S
const int   i2s_sync_word_format   = 0;           // 0 for PCM, 1 for I2S

// Custom/example properties for this example application ...

#define PROP_PRODUCT_ID      0x0101                      // Your product ID, must not be 0x0000!
#define PROP_EXAMPLE_PROPS   (PROP_PRODUCT_ID << 16)     // Base property ID value
#define PROP_EXAMPLE_VOL_MIX (PROP_EXAMPLE_PROPS+0x0001) // prop[1:5] = [volume,blend,0,0,0]
#define PROP_EXAMPLE_TONE    (PROP_EXAMPLE_PROPS+0x0002) // Coeffs, prop[1:5]=[B0,B1,B2,A1,A2]
#define PROP_EXAMPLE_IRDATA  (PROP_EXAMPLE_PROPS+0x8000) // 5 IR samples values per property

#define IR_PROP_ID(xx)  ((xx) & 0xFFFF8000) // IR property ID has two parts (ID and offset)
#define IR_PROP_IDX(xx) ((xx) & 0x00000FFF) // Up to 5*0xFFF samples (5 samples per property)

// I2C controlled 4-channel 12-bit ADC (results are returned in floating point format where
// 0 <= value < 1.0).

static void adc_read( double values[4] )
{
    byte ii, hi, lo, value;
    i2c_start( 100000 ); // Set bit clock to 400 kHz and assert start condition.
    i2c_write( 0x52+1 ); // Select I2C peripheral for Read.
    for( ii = 0; ii < 4; ++ii ) {
        hi = i2c_read(); i2c_ack(0); // Read low byte, assert ACK.
        lo = i2c_read(); i2c_ack(ii==3); // Read high byte, assert ACK (NACK on last read).
        value = (hi<<4) + (lo>>4); // Select correct value and store ADC sample.
        values[hi>>4] = ((double)value)/256.0; // Convert from byte to double (0<=val<1.0).
    }
    i2c_stop();
}

// The control task is called at a rate of 1000 Hz and should be used to implement audio CODEC
// initialization/control, pot and switch sensing via I2C ADC's, handling of properties from USB
// MIDI, and generation of properties to be consumed by the USB MIDI host and by the DSP threads.
// The incoming USB property 'rcv_prop' is valid if its ID is non-zero. Outgoing USB and DSP props
// will be sent out if their ID is non-zero - but they are only available for routing if their ID's
// are zero (non-zero indicating that routing is still in progress). It's OK to use floating point
// calculations here since this thread is not a real-time audio or DSP thread.

void make_prop( int prop[6], int id, int val1, int val2, int val3, int val4, int val5 )
{
    prop[0]=id; prop[1]=val1; prop[2]=val2; prop[3]=val3; prop[4]=val4; prop[5]=val5;
}
void copy_prop( int dst[6], const int src[6] )
{
    dst[0]=src[0]; dst[1]=src[1]; dst[2]=src[2]; dst[3]=src[3]; dst[4]=src[4]; dst[5]=src[5];
}
void copy_data( int dst[5], const int src[5] )
{
    dst[0]=src[0]; dst[1]=src[1]; dst[2]=src[2]; dst[3]=src[3]; dst[4]=src[4];
}

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    static bool intialized = 0, state = 0;

    // Initialize CODEC/ADC/DAC and other I2C-based peripherals here.
    if( !intialized ) { intialized = 1; }

    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || usb_prop[0] != 0 ) return;

    // Pass cabsim IR data to the DSP threads if usb and dsp send properties are available for use.
    if( IR_PROP_ID(rcv_prop[0]) == IR_PROP_ID(PROP_EXAMPLE_IRDATA) )
    {
        copy_prop(dsp_prop,rcv_prop); // Send to DSP threads.
        copy_prop(usb_prop,rcv_prop); // Send to USB host as an acknowledge and for flow-control.
        rcv_prop[0] = 0; // Mark the incoming USB property as consumed (allow RX of the next prop).
    }
    else // Generate property for the DSP threads if the prop buffer is free.
    {
        // Read the potentiometers -- Pot[2] is volume, Pot[1] is tone, Pot[0] is blend.
        double pot_values[4]; adc_read( pot_values );
        double volume = pot_values[2], tone = pot_values[1], blend = pot_values[0];

        // Send a CONTROL property containing knob settings (volume and blend) to the mixer thread.
        if( state == 0 ) { // State 1 is for generating and sending the VOLUME/MIX property.
            state = 1;
            dsp_prop[0] = PROP_EXAMPLE_VOL_MIX; dsp_prop[3] = dsp_prop[4] = dsp_prop[5] = 0;
            dsp_prop[1] = FQ(volume); dsp_prop[2] = FQ(blend); // float to Q28.
        }
        // Compute tone (12 dB/octave low-pass) coefficients and send them to DSP threads.
        else if( state == 1 ) { // State 2 is for generating and sending the tone coeffs property.
            state = 0;
            // This lowpass filter -3dB frequency value ranges from 1 kHz to 11 kHz via Pot[2]
            double freq = 2000.0/48000 + tone * 10000.0/48000;
            dsp_prop[0] = PROP_EXAMPLE_TONE;
            make_lowpass( dsp_prop+1, freq, 0.707 ); // Compute coefficients and populate property.
        }
    }
}

// The mixer function is called once per audio sample and is used to route USB, I2S and DSP samples.
// This function should only be used to route samples and for very basic DSP processing - not for
// substantial sample processing since this may starve the I2S audio driver. Do not use floating
// point operations since this is a real-time audio thread - all DSP operations and calculations
// should be performed using fixed-point math.
// NOTE: IIR, FIR, and BiQuad coeff and state data *must* be declared non-static global!

// Single-pole low pass filter implemented with a biquad IIR (only uses B0,B1,A1 coeffs).
int lopass_coeffs[5] = {FQ(0.999),0,0,0,0};
int lopass_stateL[4] = {0,0,0,0}, lopass_stateR[4] = {0,0,0,0}; // Initial filter state/history.

void mixer( const int* usb_output, int* usb_input,
            const int* i2s_output, int* i2s_input,
            const int* dsp_output, int* dsp_input, const int* property )
{
    static int volume = FQ(0.0), blend = FQ(0.49999); // Initial volume and blend/mix levels.

    // Convert the two ADC inputs into a single pseudo-differential mono input (mono = L - R).
    // Route the guitar signal to the USB input and to the DSP input.
    //usb_input[0] = usb_input[1] = dsp_input[0] = dsp_input[1] = i2s_output[6] - i2s_output[7];
    usb_input[0] = usb_input[1] = dsp_input[0] = dsp_input[1] = usb_output[0];

    // Check for incoming properties from USB (volume, DSP/USB audio blend, and tone control).
    if( property[0] == PROP_EXAMPLE_VOL_MIX ) { volume = property[1]; blend = property[3]; }
    // Update coeffs B1 and A1 of the low-pass biquad but leave B0=1.0,B2=0,A2=0.
    if( property[0] == PROP_EXAMPLE_TONE ) { copy_data( lopass_coeffs, &(property[2]) ); }

    // Apply tone filter to left and right DSP outputs and route this to the I2S driver inputs.
    i2s_input[6] = dsp_iir_filt( dsp_output[0], lopass_coeffs, lopass_stateL, 1 );
    i2s_input[7] = dsp_iir_filt( dsp_output[1], lopass_coeffs, lopass_stateR, 1 );

    // Blend/mix USB output audio with the processed guitar audio from the DSP output.
    i2s_input[6] = dsp_multiply( blend, i2s_input[6] ) / 2
                 + dsp_multiply( FQ(0.99999)-blend, usb_output[0] ) / 2;
    i2s_input[7] = dsp_multiply( blend, i2s_input[7] ) / 2
                 + dsp_multiply( FQ(0.99999)-blend, usb_output[1] ) / 2;

    // Apply master volume to the I2S driver inputs (i.e. to the data going to the audio DACs).
    i2s_input[6] = dsp_multiply( i2s_input[6], volume );
    i2s_input[7] = dsp_multiply( i2s_input[7], volume );

    // Bypass DSP processing (pass ADC samples to the DAC) if a switch on I/O port #0 is pressed.
    if( port_read(1) == 0 ) { i2s_input[0] = i2s_output[0]; i2s_input[1] = i2s_output[1]; }
    // Light an LED attached to I/O port #2 if the bypass switch on I/O port #1 is pressed.
    port_write( 2, port_read(1) != 0 );
}

// Audio Processing Threads. These functions run on tile 1 and are called once for each audio sample
// cycle. They cannot share data with the controller task or the mixer functions above that run on
// tile 0. The number of incoming and outgoing samples in the 'samples' array is set by the constant
// 'dsp_chan_count' defined above. Do not use floating point operations since these are real-time
// audio threads - all DSP operations and calculations should be performed using fixed-point math.
// NOTE: IIR, FIR, and BiQuad coeff and state data *must* be declared non-static global!

#define _CONVOLVE_2a(nn) \
    asm("ldd   %0,%1,%2[%3]":"=r"(b1),"=r"(b0):"r"(cc),"r"(nn)); \
    asm("ldd   %0,%1,%2[%3]":"=r"(s2),"=r"(s1):"r"(ss),"r"(nn)); \
    asm("std   %0,%1,%2[%3]"::"r"(s1), "r"(s0),"r"(ss),"r"(nn)); \
    asm("maccs %0,%1,%2,%3" :"=r"(ah),"=r"(al):"r"(b0),"r"(s0), "0"(ah),"1"(al)); \
    asm("maccs %0,%1,%2,%3" :"=r"(ah),"=r"(al):"r"(b1),"r"(s1), "0"(ah),"1"(al));

#define _CONVOLVE_2b(nn) \
    asm("ldd   %0,%1,%2[%3]":"=r"(b1),"=r"(b0):"r"(cc),"r"(nn)); \
    asm("ldd   %0,%1,%2[%3]":"=r"(s0),"=r"(s3):"r"(ss),"r"(nn)); \
    asm("std   %0,%1,%2[%3]"::"r"(s3), "r"(s2),"r"(ss),"r"(nn)); \
    asm("maccs %0,%1,%2,%3" :"=r"(ah),"=r"(al):"r"(b0),"r"(s2), "0"(ah),"1"(al)); \
    asm("maccs %0,%1,%2,%3" :"=r"(ah),"=r"(al):"r"(b1),"r"(s3), "0"(ah),"1"(al));

// 24 sample convolution using xCORE-200 assembly ("flexfx.h" doesn't support convolution).
#define _CONVOLVE_24 \
{ \
    _CONVOLVE_2a(0) _CONVOLVE_2b(1) _CONVOLVE_2a( 2) _CONVOLVE_2b( 3) \
    _CONVOLVE_2a(4) _CONVOLVE_2b(5) _CONVOLVE_2a( 6) _CONVOLVE_2b( 7) \
    _CONVOLVE_2a(8) _CONVOLVE_2b(9) _CONVOLVE_2a(10) _CONVOLVE_2b(11) \
}

// 288 sample convolution using xCORE-200 assembly ("flexfx.h" doesn't support convolution).
void _CONVOLVE_288( int* xx, int* cc, int* ss, int* ah_, int* al_ ) \
{
    int ah = *ah_, b0, b1, s0 = *xx, s1, s2, s3; unsigned al = *al_; \
    _CONVOLVE_24; cc += 24; ss += 24; _CONVOLVE_24; cc += 24; ss += 24; \
    _CONVOLVE_24; cc += 24; ss += 24; _CONVOLVE_24; cc += 24; ss += 24; \
    _CONVOLVE_24; cc += 24; ss += 24; _CONVOLVE_24; cc += 24; ss += 24; \
    _CONVOLVE_24; cc += 24; ss += 24; _CONVOLVE_24; cc += 24; ss += 24; \
    _CONVOLVE_24; cc += 24; ss += 24; _CONVOLVE_24; cc += 24; ss += 24; \
    _CONVOLVE_24; cc += 24; ss += 24; _CONVOLVE_24; cc += 24; ss += 24; \
    *xx = s0; *ah_ = ah; *al_ = al; \
}

int cabsim_coeff[2880], cabsim_state[2880]; // DSP data *must* be non-static global!

// Initialize DSP thread data (filter data and other algorithm data) here.
void dsp_initialize( void ) // Called once upon boot-up.
{
    memset( &cabsim_coeff, 0, sizeof(cabsim_coeff) );
    memset( &cabsim_state, 0, sizeof(cabsim_state) );
    cabsim_coeff[0] = cabsim_coeff[1440] = FQ(+0.9999999);
}

// Process samples (from the mixer function) and properties. Send results to stage 2.
void dsp_thread1( int* samples, const int* property )
{
    // Check for properties containing new cabsim IR data, save new data to RAM
    if( IR_PROP_ID( property[0] ) == IR_PROP_ID( PROP_EXAMPLE_IRDATA ) &&
        IR_PROP_IDX( property[0] ) > 0 && IR_PROP_IDX( property[0] ) < 2880/5 )
    {
        int offset = 5 * IR_PROP_IDX( property[0] ); // Five samples per property
        memcpy( (unsigned*) cabsim_coeff + offset, property+1, 5*sizeof(int) );
    }
    samples[2] = 0; samples[3] = 1 << 31; // Initial 64-bit Q1.63 accumulator value
    samples[4] = 0; samples[5] = 1 << 31; // Initial 64-bit Q1.63 accumulator value
    // Perform 288-sample convolution (1st 288 of 1440 total) of sample with IR data
    _CONVOLVE_288( &samples[0], cabsim_coeff+   0, cabsim_state+   0, &samples[2], &samples[3] );
    _CONVOLVE_288( &samples[1], cabsim_coeff+1440, cabsim_state+1440, &samples[4], &samples[5] );
}

// Process samples (from stage 1) and properties. Send results to stage 3.
void dsp_thread2( int* samples, const int* property )
{
    // Perform 288-sample convolution (2nd 288 of 1440 total) of sample with IR data
    _CONVOLVE_288( &samples[0], cabsim_coeff+ 288, cabsim_state+ 288, &samples[2], &samples[3] );
    _CONVOLVE_288( &samples[1], cabsim_coeff+1728, cabsim_state+1728, &samples[4], &samples[5] );
}

// Process samples (from stage 2) and properties. Send results to stage 4.
void dsp_thread3( int* samples, const int* property )
{
    // Perform 288-sample convolution (3rd 288 of 1440 total) of sample with IR data
    _CONVOLVE_288( &samples[0], cabsim_coeff+ 576, cabsim_state+ 576, &samples[2], &samples[3] );
    _CONVOLVE_288( &samples[1], cabsim_coeff+2016, cabsim_state+2016, &samples[4], &samples[5] );
}

// Process samples (from stage 3) and properties. Send results to stage 5.
void dsp_thread4( int* samples, const int* property )
{
    // Perform 288-sample convolution (4th 288 of 1440 total) of sample with IR data
    _CONVOLVE_288( &samples[0], cabsim_coeff+ 864, cabsim_state+ 864, &samples[2], &samples[3] );
    _CONVOLVE_288( &samples[1], cabsim_coeff+2304, cabsim_state+2304, &samples[4], &samples[5] );
}

// Process samples (from stage 4) and properties. Send results to the mixer function.
void dsp_thread5( int* samples, const int* property )
{
    static bool muted = 0;
    // Check IR property -- Mute at start of new IR loading, un-mute when done.
    if( IR_PROP_ID( property[0] ) == IR_PROP_ID( PROP_EXAMPLE_IRDATA ) )
    {
        if( IR_PROP_IDX( property[0] ) == 0 )        muted = 1; // First IR property -- Mute
        if( IR_PROP_IDX( property[0] ) == 2880/5-1 ) muted = 0; // Last IR property -- Un-Mute
    }
    // Perform 288-sample convolution (5th and last 288 of 1440 total) of sample with IR data
    _CONVOLVE_288( &samples[0], cabsim_coeff+1152, cabsim_state+1152, &samples[2], &samples[3] );
    _CONVOLVE_288( &samples[1], cabsim_coeff+2592, cabsim_state+2592, &samples[4], &samples[5] );
    // Extract 32-bit Q28 from 64-bit Q63 and then apply mute/un-mute based on IR loading activity.
    DSP_EXT( samples[0], samples[2], samples[3] ); samples[0] = muted ? 0 : samples[0] * 1;
    DSP_EXT( samples[1], samples[4], samples[5] ); samples[1] = muted ? 0 : samples[1] * 1;
}
