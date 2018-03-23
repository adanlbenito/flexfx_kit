#include "flexfx.h" // Defines config variables, I2C and GPIO functions, etc.
#include <math.h>   // Floating point for filter coeff calculations in the background process.
#include <string.h> // Memory and string functions

const char* product_name_string   = "FlexFX Example";   // Your product name
const char* usb_audio_output_name = "FlexFX Audio Out"; // USB audio output name
const char* usb_audio_input_name  = "FlexFX Audio In";  // USB audio input name
const char* usb_midi_output_name  = "FlexFX MIDI Out";  // USB MIDI output name
const char* usb_midi_input_name   = "FlexFX MIDI In";   // USB MIDI input name

const int audio_sample_rate     = 48000; // Audio sampling frequency
const int usb_output_chan_count = 2;     // 2 USB audio class 2.0 output channels
const int usb_input_chan_count  = 2;     // 2 USB audio class 2.0 input channels
const int i2s_channel_count     = 2;     // ADC/DAC channels per SDIN/SDOUT wire

const int i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

#define PROP_PRODUCT_ID      (0x0101)                // Your product ID, must not be 0x0000!
#define PROP_EXAMPLE_PROPS   (PROP_PRODUCT_ID << 16) // Base property ID value
#define PROP_EXAMPLE_BASS    (PROP_EXAMPLE_PROPS+1)  // Coeffs, prop[1:5]=[B0,B1,B2,A1,A2]
#define PROP_EXAMPLE_MIDDLE  (PROP_EXAMPLE_PROPS+2)  // Coeffs, prop[1:5]=[B0,B1,B2,A1,A2]
#define PROP_EXAMPLE_TREBLE  (PROP_EXAMPLE_PROPS+3)  // Coeffs, prop[1:5]=[B0,B1,B2,A1,A2]

static void read_adc( double values[4] )
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

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    static bool state = 0;

    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || dsp_prop[0] != 0 ) return;

    double pot_values[4]; read_adc( pot_values );
    double bass = pot_values[2], middle = pot_values[1], treble = pot_values[0];

    // Compute bass coefficients and send them to DSP threads.
    if( state == 0 ) { // State 2 is for generating and sending the tone coeffs property.
        state = 1;
        double gain = (bass - 0.5) * 12.0; // 0.0 < bass < 1.0 --> -12.0dB < gain < +12dB.
        dsp_prop[0] = PROP_EXAMPLE_BASS; // Compute coefficients and populate property.
        calc_lowshelf( dsp_prop+1, 100.0, 0.707, gain ); // Bass corner freq = 100 Hz.
    }
    // Compute middle coefficients and send them to DSP threads.
    else if( state == 1 ) { // State 3 is for generating and sending the tone coeffs property.
        state = 2;
        double gain = (middle - 0.5) * 12.0; // 0.0 < middle < 1.0 --> -12.0dB < gain < +12dB.
        dsp_prop[0] = PROP_EXAMPLE_MIDDLE; // Compute coefficients and populate property.
        calc_peaking( dsp_prop+1, 1000.0, 1.200, gain ); // Middle peaking freq = 10000 Hz.
    }
    // Compute treble coefficients and send them to DSP threads.
    else if( state == 2 ) { // State 3 is for generating and sending the tone coeffs property.
        state = 3;
        double gain = (treble - 0.5) * 12.0; // 0.0 < treble < 1.0 --> -12.0dB < gain < +12dB.
        dsp_prop[0] = PROP_EXAMPLE_TREBLE; // Compute coefficients and populate property.
        calc_highshelf( dsp_prop+1, 10000.0, 0.707, gain ); // Treble corner freq = 10000 Hz.
    }
    rcv_prop[0] = 0; // Mark incoming properties as 'consumed'.
}

void mixer( const int* usb_output, int* usb_input,
            const int* i2s_output, int* i2s_input,
            const int* dsp_output, int* dsp_input, const int* property )
{
    // Convert the two ADC inputs into a single pseudo-differential mono input (mono = L - R).
    int guitar_in = i2s_output[0] - i2s_output[1];
    // Route instrument input to the left USB input and to the DSP input.
    dsp_input[0] = (usb_input[0] = guitar_in) / 8; // DSP samples need to be Q28 formatted.
    // Route DSP result to the right USB input and the audio DAC.
    usb_input[1] = i2s_input[0] = i2s_input[1] = dsp_output[0] * 8; // Q28 to Q31
}

int coeff_actual[3*5], coeff_target[3*5], tone_stateL[3*4], tone_stateR[3*4];

void dsp_initialize( void ) // Called once upon boot-up.
{
    memset( coeff_actual, 0, sizeof(coeff_actual) );
    memset( coeff_target, 0, sizeof(coeff_target) );
    memset( tone_stateL,  0, sizeof(tone_stateL) );
    memset( tone_stateR,  0, sizeof(tone_stateR) );
}

void dsp_thread1( int* samples, const int* property )
{
    // Check for bass, middle or treble property data to update tone filter with.
    if( property[0] == PROP_EXAMPLE_BASS )   memcpy( coeff_target+5*0, property+1, 20 );
    if( property[0] == PROP_EXAMPLE_MIDDLE ) memcpy( coeff_target+5*1, property+1, 20 );
    if( property[0] == PROP_EXAMPLE_TREBLE ) memcpy( coeff_target+5*2, property+1, 20 );
    // Slowly adapt current tone filter biquad coeffs to their targets.
    for( int ii = 0; ii < 5; ++ii )
    {
        // Adjust coeffs for bass
        if( coeff_actual[5*0+ii] < coeff_target[5*0+ii] ) coeff_actual[5*0+ii] += FQ(+0.0001);
        if( coeff_actual[5*0+ii] > coeff_target[5*0+ii] ) coeff_actual[5*0+ii] -= FQ(+0.0001);
        // Adjust coeffs for middle
        if( coeff_actual[5*1+ii] < coeff_target[5*1+ii] ) coeff_actual[5*1+ii] += FQ(+0.0001);
        if( coeff_actual[5*1+ii] > coeff_target[5*1+ii] ) coeff_actual[5*1+ii] -= FQ(+0.0001);
        // Adjust coeffs for treble
        if( coeff_actual[5*2+ii] < coeff_target[5*2+ii] ) coeff_actual[5*2+ii] += FQ(+0.0001);
        if( coeff_actual[5*2+ii] > coeff_target[5*2+ii] ) coeff_actual[5*2+ii] -= FQ(+0.0001);
    }
    // Apply tone control to left/right audio channels.
    samples[0] = dsp_iir( samples[0], coeff_actual, tone_stateL, 3 ); // 3 cascaded Biquads
    samples[1] = dsp_iir( samples[1], coeff_actual, tone_stateR, 3 ); // 3 cascaded Biquads
}

void dsp_thread2( int* samples, const int* property )
{
}

void dsp_thread3( int* samples, const int* property )
{
}

void dsp_thread4( int* samples, const int* property )
{
}

void dsp_thread5( int* samples, const int* property )
{
}
