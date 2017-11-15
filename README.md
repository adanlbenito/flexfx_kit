FlexFX&trade; Firmware Development Kit
==================================

Application Framework
----------------------------------

The FlexFX&trade; Kit provides a light framework for developing audio processing applications running on FlexFX&trade;
hardware modules and boards.

The FlexFX&trade; Kit implements USB class 2.0 audio input and output, USB MIDI handling and routing,
up to four I2S/TDM interfaces (for multiple ADCs/DACs/CODECs), and firmware
upgrades allowing custom audio application development to remain focused on custom signal processing and
run-time algorithm control. The audio processing framework is provided in the form of object code that can be
linked with custom audio processing code resulting in a complete audio processing firmware image.

Framework Features
----------------------------------

* Simple framework for writing custom audio processing applications
* Up to 500 MIPs available for signal processing algorithms (FlexFX module with XUF216)
* Up to 1500 MIPs available for signal processing algorithms (FlexFX module with XEF232)
* 32/64 bit fixed point DSP support, single-cycle instructions
* Up to 32x32 channel USB audio, up to 384 kHz audio sample rate
* USB interface for USB audio streaming ad USB MIDI for effects control and firmware updating
* Functions for I2C bus (for peripheral control) and port input/output
* I2S supports up to 8-slot TDM and up to four separate ADCs and four separate DACs

Functional Organization
----------------------------------

![alt tag](https://raw.githubusercontent.com/markseel/flexfx_kit/master/flexfx.png)

Grey - User code for custom audio processing and property handling <br/>
Blue - Realtime DSP threads, 100 MIPs each <br/>
Green - Program and data RAM, shared by all DSP threads <br/>
Yellow - Control (non-realtime) and audio mixer (realtime) threads <br/>
Red - USB, I2S, I/O ports, and I2C interfaces <br/>

Minimum Application
----------------------------------

```C
#include "main.h" // Defines config variables, I2C and GPIO functions, DSP functions, etc.

// System configuration ...

const char* company_name_string    = "BitStream"; // Your company name
const char* product_name_string    = "FlexFX";    // Your product name
const int   default_sample_rate    = 48000;       // Default sample rate at boot-up
const int   supported_sample_rates = 0x02;        // 1=44k1, 2=48k0, 4=88k2, ... 32=192k0
const int   dsp_in_out_chan_count  = 32;          // 32 channels to/from each DSP thread
const int   usb_output_chan_count  = 2;           // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;           // 2 USB audio class 2.0 input channels
const int   i2s_tdm_slot_count     = 2;           // 2 for I2S (Stereo), 4 for I4S, 8 = I8S
const int   i2s_sync_word_format   = 0;           // 0 for PCM, 1 for I2S

// The control task is called at a rate of 1000 Hz and should be used to implement audio CODEC
// initialization/control, pot and switch sensing via I2C ADC's, handling of properties from USB
// MIDI, and generation of properties to be consumed by the USB MIDI host and by the DSP threads.

void control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
}

// The mixer function is called once per audio sample and is used to route USB, I2S and DSP samples.
// This function should only be used to route samples and for very basic DSP processing - not for
// substantial sample processing since this may starve the audio driver.

void mixer( const int* usb_output, int* usb_input,
            const int* i2s_output, int* i2s_input,
            const int* dsp_output, int* dsp_input, const int* property )
{
}

// Audio Processing Threads.  These functions run on tile 1 and are called once for each audio
// sample cycle.  They cannot share data with the controller task or the mixer functions above
// that run on tile 0.  The number of incoming and outgoing samples in the 'samples' array is set
// by the constant 'dsp_chan_count' defined above.  NOTE: IIR, FIR, and BiQuad coeff and state
// data *must* be declared non-static global!

void dsp_initialize( void ) // Called once upon boot-up.
{
}

void dsp_thread1( int* samples, const int* property )
{
    // Process samples (from mixer) and properties. Send DSP results to stage 2.
}

void dsp_thread2( int* samples, const int* property )
{
    // Process samples (from stage 1) and properties. Send DSP results to stage 3.
}
void dsp_thread3( int* samples, const int* property )
{
    // Process samples (from stage 2) and properties. Send DSP results to stage 4.
}
void dsp_thread4( int* samples, const int* property )
{
    // Process samples (from stage 3) and properties. Send DSP results to stage 5.
}
void dsp_thread5( int* samples, const int* property )
{
    // Process samples (from stage 4) and properties. Send DSP results to the mixer function.
}
```

Programming Interface
----------------------------------

```C
#include <platform.h>

typedef unsigned char byte;
typedef unsigned int  bool;

// Flash memory functions for data persistance (do not use these in real-time DSP threads).

extern void flash_open ( void );                   // Open the flash device
extern void flash_seek ( int page_number );        // Set the page for the next write/read
extern void flash_read ( byte buffer[256] );       // Read next flash page, move to the next page
extern void flash_write( const byte buffer[256] ); // Write next flash page, move to the next page
extern void flash_close( void );                   // Close the flash device

// Port I/O functions ...

extern void port_write( int portnum, bool value ); // Write binary value to port
extern bool port_read ( int portnum );             // Read binary value from port

// I2C functions for peripheral control (do not use these in real-time DSP threads).

extern void i2c_start( int speed );    // Set bit rate, assert an I2C start condition.
extern byte i2c_write( byte value );   // Write 8-bit data value.
extern byte i2c_read ( void );         // Read 8-bit data value.
extern void i2c_ack  ( byte ack );     // Assert the ACK/NACK bit after a read.
extern void i2c_stop ( void );         // Assert an I2C stop condition.

// FQ converts Q28 fixed-point to floating point
// QF converts floating-point to Q28 fixed-point

#define FQ(hh) (((hh)<0.0)?((int)((double)(1<<28)*(hh)-0.5)):((int)(((double)(1<<28)-1)*(hh)+0.5)))
#define QF(xx) (((int)(xx)<0)?((double)(int)(xx))/(1<<28):((double)(xx))/((1<<28)-1))

// MAC performs 32x32 multiply and 64-bit accumulation, SAT saturates a 64-bit result, EXT converts
// a 64-bit result to a 32-bit value (extract 32 from 64), LD2/ST2 loads/stores two 32-values
// from/to 64-bit aligned 32-bit data arrays at address PP. All 32-bit fixed-point values are Q28
// fixed-point formatted.
//
// AH (high) and AL (low) form the 64-bit signed accumulator
// XX, YY, and AA are 32-bit Q28 fixed point values
// PP is a 64-bit aligned pointer to two 32-bit Q31 values

#define DSP_MAC( ah, al, xx, yy ) asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(yy),"0"(ah),"1"(al) );
#define DSP_SAT( ah, al )         asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(28),"0"(ah),"1"(al));
#define DSP_EXT( xx, ah, al )     asm("lextract %0,%1,%2,%3,32":"=r"(xx):"r"(ah),"r"(al),"r"(28));
#define DSP_LD2( pp, xx, yy )     asm("ldd %0,%1,%2[0]":"=r"(xx),"=r"(yy):"r"(pp));
#define DSP_ST2( pp, xx, yy )     asm("std %0,%1,%2[0]"::"r"(xx), "r"(yy),"r"(pp));

inline int dsp_multiply( int xx, int yy ) // RR = XX * YY
{
    int ah = 0; unsigned al = 1<<(28-1);
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(yy),"0"(ah),"1"(al) );
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(28));
    return ah;
}
inline int dsp_mult_acc( int xx, int yy, int aa ) // RR = XX * YY + AA
{
    int ah = aa; unsigned al = 0;
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(yy),"0"(ah),"1"(al) );
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(28));
    return ah;
}

// Math and filter functions.
//
// XX, CC, SS, Yn, and AA are 32-bit fixed point samples/data in Q28 format
// DD is the distance (0<=DD<1) between the first two points for interpolation
// Yn are the data points to be interpolated
// NN is FIR filter tap-count for 'fir', 'upsample', 'dnsample' and 'convolve' functions
// NN is IIR filter order or or IIR filter count for cascaded IIR's
// CC is array of 32-bit filter coefficients - length is 'nn' for FIR, nn * 5 for IIR
// SS is array of 32-bit filter state - length is 'nn' for FIR, nn * 4 for IIR, 1 for LPF/HPF/INT
// KK is time constant for LPF, HPF and INT where KK = exp(-2.0 * PI * Fc)

int dsp_random   ( int gg, int seed );                       // Random number, gg = previous value
int dsp_integrate( int xx, int kk, int* ss );                // Leaky integrator
int dsp_fir_filt ( int xx, const int* cc, int* ss, int nn ); // FIR filter of nn taps
int dsp_iir_filt ( int xx, const int* cc, int* ss, int nn ); // nn Cascaded bi-quad IIR filters
int dsp_interp   ( int dd, int y1, int y2 );                 // Linear interpolation
int dsp_lagrange ( int dd, int y1, int y2, int y3 );         // Lagrange interpolation
int dsp_volume   ( int xx, int volume );                     // Noise-shaped volume control

// Biquad filter coefficient calculation functions (do not use these in real-time DSP threads).
//
// CC is an array of floating point filter coefficients
// FF, QQ, GG are the filter frequency, filter Q-factor, and filter gain
// For low/high pass filters set QQ to zero to create a single-pole 6db/octave filter

void make_notch    ( int cc[5], double ff, double qq );
void make_lowpass  ( int cc[5], double ff, double qq );
void make_highpass ( int cc[5], double ff, double qq );
void make_allpass  ( int cc[5], double ff, double qq );
void make_bandpass ( int cc[5], double ff1, double ff2 );
void make_peaking  ( int cc[5], double ff, double qq, double gg );
void make_lowshelf ( int cc[5], double ff, double qq, double gg );
void make_highshelf( int cc[5], double ff, double qq, double gg );
```

Custom Applications
----------------------------------

One can create custom audio processing effects by downloading the FlexFX&trade; audio processing framework, adding custom audio processing DSP code and property handling code, and then compiling and linking using XMOS tools (xTIMEcomposer, free to download).
The custom firmware can then be burned to FLASH using xTIMEcomposer and the XTAG-2 or XTAG-3 JTAG board ($20 from Digikey),
via USB/MIDI (there are special properties defined for firmware upgrading and boot image selection).

FlexFX&trade; implements the USB class 2.0 audio and USB MIDI interfaces, the I2S/CODEC interface, sample data transfer from USB/I2S/DSP threads, property/parameter routing, and firmware upgrading.

All one has to do then is add DSP code and any relevant custom property handling code to the five audio processing threads which all run in parallel and implement a 5-stage audio processing pipeline.
Each processing thread is allocated approximately 100 MIPS and executes once per audio cycle.
All threads are executed once per audio cycle and therefore operate on USB and I2S sample data one audio frame at a time.

Put these functions in a sourcefile with the extension '.c' and compile as described above.
See 'example.c' for more information.

Development Steps
----------------------------------

1) Download and install the free XMOS development tools from www.xmos.com
2) Obtain the FlexFX&trade; dev-kit for building your own apps from https://github.com/markseel/flexfx_kit
3) Set an environment path to point to XMOS command-line build tools.
4) Add your own source code to a new 'C' file (e.g. ('myapp.c')
5) Build the application (the build script uses the XMOS command line compiler/linker).
6) Load the application into RAM and run it using the XMOS JTAG board.
7) Burn firmware application to FLASH using the XMOS JTAG board.
8) Convert the firmware application to a binary image for burning FLASH via USB.
9) Burn the firmware binary via USB using the 'flexfx.py' Python script.

```
Get the kit ............ git clone https://github.com/markseel/flexfx_kit.git
Set environment vars
  on Windows ........... c:\Program Files (x86)\XMOS\xTIMEcomposer\Community_14.2.0\SetEnv.bat
  on OS X / Linux ...... /Applications/XMOS_xTIMEcomposer_Community_14.1.1/SetEnv.command
Build your app ......... build.bat myapp
Run the application .... xrun --xscope example.xe
Burn to FLASH (XTAG) ... xflash example.xe
Convert to binary ...... xflash --noinq --boot-partition-size 524288 example.xe -o example.bin
Burn to FLASH (USB) .... flexfx.py 0 example.bin
```

Run-time Control
----------------------------------

Audio processing code is controlled by properties.
A property is composed of six 32-bit words where the first word is the property ID and the remaining five 32-bit words are property data.
Properties can be sent to and received from audio processing code by being encapsulated in MIDI SYSEX messages.
The FlexFX SDK handles USB MIDI and MIDI sysex rendering/parsing -
the audio firmware only sees six 32-word properties and is not aware of USB MIDI and sysex rendering/parsing as this is handled by the FlexFX SDK.

An example property is shown below:

```
Property ID = 0x00001301
Param 1     = 0x11223344
Param 2     = 0x55667788
Param 3     = 0x99aabbcc
Param 4     = 0x01234567
Param 5     = 0x89abcdef
```

For detailed information regarding the rendering/parsing process and MIDI SYSEX formatting see the 'flexfx.py' script
that's used to send/receive properties to FlexFX applications via USB.

Example Application
----------------------------------

Stereo Cabinet Simulation with Tone/Volume and USB Audio Mixing.

```C
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
```

USB Host Application
----------------------------------

The Python script 'flexfx.py' implements various host functions for testing FlexFX firmware via USB.

#### Usage #1
Display attached USB MIDI devices.  The FlexFX DSP board enumerates as device #0 in this example.
```
bash$ python flexfx.py
MIDI Output Devices: 0='FlexFX'
MIDI Input Devices:  0='FlexFX'
```

#### Usage #2
Indefinately display properties being sent from the DSP board, enumerated as USB MIDI device #0, to the USB host (CRTL-C to terminate).  The first six columns are the 32-bit property ID and five property values printed in hex/ASCII.  The last five columns are thge five property values converted from Q28 fixed-point to floating point).  These rows are printed at a very high rate - as fast as Python can obtain the USB data over MIDI and print to the console. 
```
bash$ python flexfx.py 0
11111111  0478ee7b 08f1dcf7 0478ee7b 00dcd765 fd3f6eac  +0.27952 +0.55905 +0.27952 +0.05392 -0.17201
11111111  0472eb5b 08e5d6b6 0472eb5b 00f5625e fd3ef034  +0.27806 +0.55611 +0.27806 +0.05991 -0.17213
11111111  0472eb5b 08e5d6b6 0472eb5b 00f5625e fd3ef034  +0.27806 +0.55611 +0.27806 +0.05991 -0.17213
11111111  0478ee7b 08f1dcf7 0478ee7b 00dcd765 fd3f6eac  +0.27952 +0.55905 +0.27952 +0.05392 -0.17201
...
```

#### Usage #3
Burn a custom firmware application to the DSP board's FLASH memory (board is enumerated as MIDI device #0).  This takes about 10 seconds.
```
bash$ python flexfx.py 0 app_cabsim.bin
.....................................................................................Done.
```

#### Usage #4
Load impulse response data, stored in a WAV file, to the cabinet simulator example running on the DSP board that is enumerated as MIDI device #0.  This takes less than one second.
```
bash$ python flexfx.py 0 your_IR_file.wav
Done.
```

Design Scripts
----------------------------------

There are four Python scripts to aid with algorithm and filter design.  The FIR and IIR design scripts require that the Python packeges 'scipy' and 'matplotlib' are installed.

#### FIR Filter Design Script

```
bash-3.2$ python util_fir.py
Usage: python util_fir <pass_freq> <stop_freq> <ripple> <attenuation>
       <pass_freq> is the passband frequency relative to Fs (0 <= freq < 0.5)
       <stop_freq> is the stopband frequency relative to Fs (0 <= freq < 0.5)
       <ripple> is the maximum passband ripple
       <attenuation> is the stopband attenuation
```

'util_fir.py' is used to generate filter coefficients and to plot the filter response.  These coefficients can be used directly in your custom application 'C' file since the FQ macro converts floating point to Q28 fixed point format required by FlexFX DSP functions.  For this example the filter passband frequency is 0.2 (e.g. 9.6kHz @ 48 kHz Fs), a stopband frequency of 0.3 (e.g. 14.4 kHz @ 48 kHz Fs), a maximum passband ripple of 1.0 dB, and a stopband attenuation of -60 dB.  This script will display the magnitude response and the impulse response, and then print out the 38 filter coefficients for the resulting 38-tap FIR filter.

```
bash-3.2$ python util_fir.py 0.2 0.3 1.0 -60       
FQ(-0.000248008),FQ(+0.000533374),FQ(+0.000955507),FQ(-0.001549687),FQ(-0.002356083),FQ(+0.003420655),FQ(+0.004796811),FQ(-0.006548327),
FQ(-0.008754448),FQ(+0.011518777),FQ(+0.014985062),FQ(-0.019366261),FQ(-0.025001053),FQ(+0.032472519),FQ(+0.042885423),FQ(-0.058618855),
FQ(-0.085884667),FQ(+0.147517761),FQ(+0.449241500),FQ(+0.449241500),FQ(+0.147517761),FQ(-0.085884667),FQ(-0.058618855),FQ(+0.042885423),
FQ(+0.032472519),FQ(-0.025001053),FQ(-0.019366261),FQ(+0.014985062),FQ(+0.011518777),FQ(-0.008754448),FQ(-0.006548327),FQ(+0.004796811),
FQ(+0.003420655),FQ(-0.002356083),FQ(-0.001549687),FQ(+0.000955507),FQ(+0.000533374),FQ(-0.000248008)
```
![alt tag](https://raw.githubusercontent.com/markseel/flexfx_kit/master/util_fir.png)

#### IIR Filter Design Script

```
bash$ python util_iir.py
Usage: python design.py <type> <freq> <Q> <gain>
       <type> filter type (notch, lowpass, highpass, allpass, bandpass, peaking, highshelf, or lowshelf
       <freq> is cutoff frequency relative to Fs (0 <= freq < 0.5)
       <Q> is the filter Q-factor
       <gain> is the filter positive or negative gain in dB
```

'util_iir.py' is used to generate biquad IIR filter coefficients (B0, B1, B2, A1, and A2) and to plot the filter response.  These coefficients can be used directly in your custom application 'C' file since the FQ macro converts floating point to Q28 fixed point format required by FlexFX DSP functions.  For this example a bandpass filter and a peaking filter were designed.  This script will display the magnitude and phase responses and then print out the 38 filter coefficients for the resulting 38-tap FIR filter.

```
bash$ python util_iir.py bandpass 0.25 0.707 12.0
FQ(+0.399564099),FQ(+0.000000000),FQ(-0.399564099),FQ(-0.000000000),FQ(+0.598256395)

bash-3.2$ python util_iir.py peaking 0.2 2.5 12.0       
FQ(+1.259455676),FQ(-0.564243794),FQ(+0.566475599),FQ(-0.564243794),FQ(+0.825931275)
```
![alt tag](https://raw.githubusercontent.com/markseel/flexfx_kit/master/util_iir.png)

#### WAVE File Parsing Script

'util_wave.py' is used to parse one or more WAVE files and print the WAVE file sample values to the console in floating point format.

```
bash$ python util_wave.py ir1.wav ir2.wav
-0.00028253 +0.00012267 
+0.00050592 -0.00001955 
+0.00374091 +0.00060725 
+0.01383054 +0.00131202 
+0.03626263 +0.00480974 
+0.08285010 +0.01186383 
+0.16732728 +0.03065085 
+0.30201840 +0.08263147 
+0.47006404 +0.20321691 
+0.64849544 +0.41884899 
+0.81581724 +0.72721589 
+0.94237792 +0.97999990 
+0.92178667 +0.96471488 
+0.72284126 +0.66369593 
+0.34437060 +0.25017858 
-0.06371593 -0.16284466 
-0.39860737 -0.51766562 
...
```

#### Data Plotting Script

```
bash$ python util_plot.py
Usage: python plot.py <datafile> time
       python plot.py <datafile> time [beg end]
       python plot.py <datafile> freq lin
       python plot.py <datafile> freq log
Where: <datafile> Contains one sample value per line.  Each sample is an
                  ASCII/HEX value (e.g. FFFF0001) representing a fixed-
                  point sample value.
       time       Indicates that a time-domain plot should be shown
       freq       Indicates that a frequency-domain plot should be shown
       [beg end]  Optional; specifies the first and last sample in order to
                  create a sub-set of data to be plotted
Examle: Create time-domain plot data in 'out.txt' showing samples 100
        through 300 ... bash$ python plot.py out.txt 100 300
Examle: Create frequency-domain plot data in 'out.txt' showing the Y-axis
        with a logarithmic scale ... bash$ python plot.py out.txt freq log
```

The 'util_plot.py' script graphs data contained in a data file which can contan an arbitrary number of data columns of floating point values.  The data can be plotted in the time domain or in the frequency domain.  In this example two cabinet simulation data files are plotted to show their frequency domain impulse responses (i.e the cabinet frequency responses).

```
bash$ python util_wave.py ir1.wav ir2.wav > output.txt
bash$ python util_plot.py output.txt freq log
bash$ python util_plot.py output.txt time 0 150
```
![alt tag](https://raw.githubusercontent.com/markseel/flexfx_kit/master/util_plot.png)
