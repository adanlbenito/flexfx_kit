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

Comments and questions are welcome! flexfx@protonmail.com

Framework Features
----------------------------------

* Simple framework for writing custom audio processing applications
* Up to 500 MIPs available for signal processing algorithms (FlexFX module with XUF216)
* Up to 1500 MIPs available for signal processing algorithms (FlexFX module with XEF232)
* 32/64 bit fixed point DSP support, single-cycle instructions
* Up to 32x32 (48 kHz) channels of USB and I2S audio, up to 192 kHz audio sample rate at 8x8
* Single audio cycle DSP processing for all 32 channels (e.g. audio block size = 1)
* System latency (ADC I2S to DSP to DAC I2S) of 16 audio cycles (16/Fs). 
* USB interface for USB audio streaming ad USB MIDI for effects control and firmware updating
* Functions for I2C bus (for peripheral control) and port input/output
* I2S supports up to 8-slot TDM and up to four separate ADCs and four separate DACs

Compatible Hardware
----------------------------------
![alt tag](https://raw.githubusercontent.com/markseel/flexfx_kit/master/hardware.png)

Firmware Architecture
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

const char* product_name_string    = "FlexFX";    // Your company/product name
const int   default_sample_rate    = 48000;       // Default sample rate at boot-up
const int   supported_sample_rates = 0x02;        // 1=44k1, 2=48k0, 4=88k2, ... 32=192k0
const int   dsp_in_out_chan_count  = 32;          // 32 channels to/from each DSP thread
const int   usb_output_chan_count  = 2;           // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;           // 2 USB audio class 2.0 input channels
const int   i2s_tdm_slot_count     = 2;           // 2 for I2S (Stereo), 4 for I4S, 8 = I8S
const int   i2s_sync_word_format   = 0;           // 0 for PCM, 1 for I2S
const char  interface_text[]       = "No interface is specified";
const char  controller_app[]       = "No controller is available";

// The control task is called at a rate of 1000 Hz and should be used to implement audio CODEC
// initialization/control, pot and switch sensing via I2C ADC's, handling of properties from USB
// MIDI, and generation of properties to be consumed by the USB MIDI host and by the DSP threads.

void app_control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
}

// The mixer function is called once per audio sample and is used to route USB, I2S and DSP samples.
// This function should only be used to route samples and for very basic DSP processing - not for
// substantial sample processing since this may starve the audio driver.

void app_mixer( const int usb_output[32], int usb_input[32],
                const int i2s_output[32], int i2s_input[32],
                const int dsp_output[32], int dsp_input[32], const int property[6] )
{
}

// Audio Processing Threads.  These functions run on tile 1 and are called once for each audio
// sample cycle.  They cannot share data with the controller task or the mixer functions above
// that run on tile 0.  The number of incoming and outgoing samples in the 'samples' array is set
// by the constant 'dsp_chan_count' defined above.  NOTE: IIR, FIR, and BiQuad coeff and state
// data *must* be declared non-static global!

void app_thread1( int samples[32], const int property[6] )
{
    // Process samples (from mixer) and properties. Send DSP results to stage 2.
}

void app_thread2( int samples[32], const int property[6] )
{
    // Process samples (from stage 1) and properties. Send DSP results to stage 3.
}
void app_thread3( int samples[32], const int property[6] )
{
    // Process samples (from stage 2) and properties. Send DSP results to stage 4.
}
void app_thread4( int samples[32], const int property[6] )
{
    // Process samples (from stage 3) and properties. Send DSP results to stage 5.
}
void app_thread5( int samples[32], const int property[6] )
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

// Port and pin I/O functions. DAC/ADC port reads/writes will disable I2S/TDM!

extern void io_write( int pin_num, bool value ); // Write binary value to IO pin
extern bool io_read ( int pin_num );             // Read binary value from IO pin

extern void port_write( byte value ); // Write 4-bit nibble to DAC port (dac0-dac3 pins)
extern byte port_read ( void );       // Read 4-bit nibble from ADC port (adc0-adc3 pins)

// I2C functions for peripheral control (do not use these in real-time DSP threads).

extern void i2c_start( int speed );    // Set bit rate, assert an I2C start condition.
extern byte i2c_write( byte value );   // Write 8-bit data value.
extern byte i2c_read ( void );         // Read 8-bit data value.
extern void i2c_ack  ( byte ack );     // Assert the ACK/NACK bit after a read.
extern void i2c_stop ( void );         // Assert an I2C stop condition.

// FQ converts Q28 fixed-point to floating point
// QF converts floating-point to Q28 fixed-point

#define FQ(hh) (((hh)<0.0)?((int)((double)(1u<<28)*(hh)-0.5)):((int)(((double)(1u<<28)-1)*(hh)+0.5)))
#define QF(xx) (((int)(xx)<0)?((double)(int)(xx))/(1u<<28):((double)(xx))/((1u<<28)-1))

// MAC performs 32x32 multiply and 64-bit accumulation, SAT saturates a 64-bit result, EXT converts
// a 64-bit result to a 32-bit value (extract 32 from 64), LD2/ST2 loads/stores two 32-values
// from/to 64-bit aligned 32-bit data arrays at address PP. All 32-bit fixed-point values are Q28
// fixed-point formatted.
//
// AH (high) and AL (low) form the 64-bit signed accumulator
// XX, YY, and AA are 32-bit Q28 fixed point values
// PP is a 64-bit aligned pointer to two 32-bit Q28 values

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

// Lookup tables for sine, various sigmoid functions, logarithm, etc. Sigmoid functions have slope
// of 8 at x=0, a minimum value of 0.0 at x=0, and maximim value approaching 1.0 as x approaches
// 1.0. Tables indexed by II are NN in length and consist of 32-bit Q28 fixed point values.
// All table values follow this rule: 0.0 <= xx < 1.0 where ii = int(xx*nn) --> 0.0 <= yy < 1.0.
// SINE(xx) = 0.5+sin(2pi*xx)/2, ATAN(xx) = c*atan(xx*8), TANH(xx) = tanh(xx*8),
// NEXP(xx) = 1-e^(-xx*8), RLOG(xx) = -log10(1.0-xx)/8,

extern int dsp_sine_08[256], dsp_sine_10[1024], dsp_sine_12[4096], dsp_sine_14[16384];
extern int dsp_atan_08[256], dsp_atan_10[1024], dsp_atan_12[4096], dsp_atan_14[16384];
extern int dsp_tanh_08[256], dsp_tanh_10[1024], dsp_tanh_12[4096], dsp_tanh_14[16384];
extern int dsp_nexp_08[256], dsp_nexp_10[1024], dsp_nexp_12[4096], dsp_nexp_14[16384];
extern int dsp_rlog_08[256], dsp_rlog_10[1024], dsp_rlog_12[4096], dsp_rlog_14[16384];

// Math and filter functions.
//
// XX, CC, SS, Yn, MM, and AA are 32-bit fixed point samples/data in Q28 format
// DD is the distance (0<=DD<1) between the first two points for interpolation
// KK is a time constant, Q28 format
// Yn are the data points to be interpolated
// NN is FIR filter tap-count for 'fir', 'upsample', 'dnsample' and 'convolve' functions
// NN is IIR filter order or or IIR filter count for cascaded IIR's
// NN is number of samples in XX for scalar and vector math functions
// CC is array of 32-bit filter coefficients - length is 'nn' for FIR, nn * 5 for IIR
// SS is array of 32-bit filter state - length is 'nn' for FIR, nn * 4 for IIR, 3 for DCBLOCK
// RR is the up-sampling/interpolation or down-sampling/decimation ratio
// AH (high) and AL (low) form the 64-bit signed accumulator

int  dsp_random  ( int  gg, int seed );               // Random number, gg = previous value
int  dsp_blend   ( int  xx, int yy, int mm );         // 0.0 (100% xx) <= mm <= 1.0 (100% yy)
int  dsp_interp  ( int  dd, int y1, int y2 );         // 1st order (linear) interpolation
int  dsp_lagrange( int  dd, int y1, int y2, int y3 ); // 2nd order (Lagrange) interpolation

int  dsp_dcblock ( int  xx, int* ss, int kk ); // DC blocker
int  dsp_envelope( int  xx, int* ss, int kk ); // Envelope detector,vo=vi*(1–e^(–t/RC)),kk=2*RC/Fs

int  dsp_sum     ( int* xx, int nn );          // sum(xx[0:N-1])
int  dsp_abs_sum ( int* xx, int nn );          // sum(abs(xx[0:N-1]))
int  dsp_power   ( int* xx, int nn );          // xx[0:N-1]^2
int  dsp_sca_sqrt( int  xx );                  // xx ^ 0.5
void dsp_vec_sqrt( int* xx, int nn );          // xx[0:N-1] = xx[0:N-1] ^ 0.5
void dsp_abs     ( int* xx, int nn );          // xx[0:N-1] = abs(xx[0:N-1])
void dsp_square  ( int* xx, int nn );          // xx[0:N-1] = xx[0:N-1]^2
void dsp_sca_add ( int* xx, int  kk, int nn ); // xx[0:N-1] += kk
void dsp_sca_mult( int* xx, int  kk, int nn ); // xx[0:N-1] *= kk
void dsp_vec_add ( int* xx, int* yy, int nn ); // xx[0:N-1] = xx[0:N-1] + yy[0:N-1]
void dsp_vec_mult( int* xx, int* yy, int nn ); // xx[0:N-1] = xx[0:N-1] * yy[0:N-1]

int  dsp_convolve( int  xx, const int* cc, int* ss, int* ah, int* al ); // 240 tap FIR convolution
int  dsp_iir     ( int  xx, const int* cc, int* ss, int nn ); // nn Cascaded bi-quad IIR filters
int  dsp_fir     ( int  xx, const int* cc, int* ss, int nn ); // FIR filter of nn taps
void dsp_fir_up  ( int* xx, const int* cc, int* ss, int nn, int rr ); // FIR up-sampling/interpolation
void dsp_fir_dn  ( int* xx, const int* cc, int* ss, int nn, int rr ); // FIR dn-sampling/decimation
void dsp_cic_up  ( int* xx, const int* cc, int* ss, int nn, int rr ); // CIC up-sampling/interpolation
void dsp_cic_dn  ( int* xx, const int* cc, int* ss, int nn, int rr ); // CIC dn-sampling/decimation

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
4) Add your own source code to a new 'C' file (e.g. ('your_application.c')
5) Build the application using the build script / batch file (uses the XMOS command line compiler/linker).
6) Burn the firmware binary via USB using the 'flexfx.py' Python script.

```
Get the kit ............... git clone https://github.com/markseel/flexfx_kit.git
Set XMOS build tools environment variables
  on Windows .............. c:\Program Files (x86)\XMOS\xTIMEcomposer\Community_14.2.0\SetEnv.bat
  on OS X / Linux ......... /Applications/XMOS_xTIMEcomposer_Community_14.1.1/SetEnv.command
Build your custom application (be sure to exclude the .c file extension)
  on Windows .............. build.bat your_application
  on OS X / Linux ......... ./build.sh your_application
Burn to FLASH via USB ..... flexfx.py 0 your_application.bin
```

Run-time Control
----------------------------------

FlexFX applications can be controlled using FlexFX property exchanges via USB MIDI.
A property is composed of a 16-bit IDC and five 32-bit data words for a total of 44 bytes of data.

An example property is shown below:

```
Property ID = 0x01011301     The ID must have a non-zero 16-bit upper word (0x0101 in this example)
Param 1     = 0x11223344
Param 2     = 0x55667788
Param 3     = 0x99aabbcc
Param 4     = 0x01234567
Param 5     = 0x89abcdef
```

Since FlexFX properties are transfered via MIDI the 44 bytes of data must be encapsulated withing a MIDI SYSEX message when
parsing and rendering FlexFX properties on the USB host.
The FlexFX framework handles parsing and rendering of MIDI SYSEX encapulated FlexFX data transfer therefore the user
application need not deal with MIDi SYSEX. The FlexFX SDK handles the USB MIDI and MIDI sysex rendering/parsing -
the audio firmware only sees 16-bit ID and five 32-word properties and is not aware of USB MIDI and sysex rendering/parsing as this is handled by the FlexFX SDK.

For detailed information regarding the rendering/parsing process and MIDI SYSEX formatting see the 'flexfx.py' script
that's used to send/receive properties to FlexFX applications via USB.

FlexFX supports predeifned properties with the 16-bit ID being less than 0x1000.
User defined properties should therefore use 16-bit ID's greater then or equal to 0x1000.

```
ID        DIRECTION        SUMMARY
0001      Bidirectional    Return 3DEGFLEX, versions and props[4:5] to host
0002      Bidirectional    Return the device product name (up to 40 bytes)
0003      Device to Host   Start dumping the text in this file
0004      Bidirectional    Next 40 bytes of property interface text
0005      Host to Device   End of property interface text dump
0006      Device to Host   Start a controller javascript code dump
0007      Bidirectional    Next 40 bytes of javascript code text
0008      Host to Device   End of controller javascript code text
0009      Bidirectional    Begin firmware upgrade, echoed back to host
000A      Bidirectional    Next 32 bytes of firmware image data, echoed
000B      Bidirectional    End firmware upgrade, echoed back to host
000C      Bidirectional    Begin flash user data load, echoed back to host
000D      Bidirectional    Next 32 bytes of flash user data, echoed
000E      Bidirectional    End flash data loading, echoed back to host
000F      Bidirectional    Query/return DSP thread run-time (audio cycles)
```

#### FlexFX ID = 0x0001
#### FlexFX ID = 0x0002
#### FlexFX ID = 0x0003
#### FlexFX ID = 0x0004
#### FlexFX ID = 0x0005
#### FlexFX ID = 0x0006
#### FlexFX ID = 0x0007
#### FlexFX ID = 0x0008
#### FlexFX ID = 0x0009
#### FlexFX ID = 0x000A
#### FlexFX ID = 0x000B
#### FlexFX ID = 0x000C
#### FlexFX ID = 0x000D
#### FlexFX ID = 0x000E
#### FlexFX ID = 0x000F

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
Indefinitely display properties being sent from the DSP board, enumerated as USB MIDI device #0, to the USB host (CRTL-C to terminate).  The first six columns are the 32-bit property ID and five property values printed in hex/ASCII.  The last five columns are the same five property values converted from Q28 fixed-point to floating point.  These rows are printed at a very high rate - as fast as Python can obtain the USB data over MIDI and print to the console. 
```
bash$ python flexfx.py 0
11111111  0478ee7b 08f1dcf7 0478ee7b 00dcd765 fd3f6eac  +0.27952 +0.55905 +0.27952 +0.05392 -0.17201
11111111  0472eb5b 08e5d6b6 0472eb5b 00f5625e fd3ef034  +0.27806 +0.55611 +0.27806 +0.05991 -0.17213
11111111  0472eb5b 08e5d6b6 0472eb5b 00f5625e fd3ef034  +0.27806 +0.55611 +0.27806 +0.05991 -0.17213
11111111  0478ee7b 08f1dcf7 0478ee7b 00dcd765 fd3f6eac  +0.27952 +0.55905 +0.27952 +0.05392 -0.17201
...
```
This video shows the 'flexfx.py' script receiving properties from the DSP board and printing them to the console.
For this example FlexFX firmware is capturing four potentiometer values, packaging them up into a property,
and sending the property to the USB host (see code below).
One of the pots is being turned back and fourth resulting in changes to the corresponding property value.
https://raw.githubusercontent.com/markseel/flexfx_kit/master/flexfx.py.usage2.mp4
```
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
void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || usb_prop[0] != 0 ) return;
    // Read the potentiometers -- convert the values from float to Q28 using the FQ macro.
    double values[4]; adc_read( values );
    // Create property with three Q28 pot values to send to USB host
    usb_prop[0] = 0x01010000; dsp_prop[4] = dsp_prop[5] = 0;
    usb_prop[1] = FQ(values[0]); usb_prop[2] = FQ(values[1]); usb_prop[3] = FQ(values[2]);
}
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
int coeffs[38] = 
{	
    FQ(-0.000248008),FQ(+0.000533374),FQ(+0.000955507),FQ(-0.001549687),FQ(-0.002356083),
    FQ(+0.003420655),FQ(+0.004796811),FQ(-0.006548327),FQ(-0.008754448),FQ(+0.011518777),
    FQ(+0.014985062),FQ(-0.019366261),FQ(-0.025001053),FQ(+0.032472519),FQ(+0.042885423),
    FQ(-0.058618855),FQ(-0.085884667),FQ(+0.147517761),FQ(+0.449241500),FQ(+0.449241500),
    FQ(+0.147517761),FQ(-0.085884667),FQ(-0.058618855),FQ(+0.042885423),FQ(+0.032472519),
    FQ(-0.025001053),FQ(-0.019366261),FQ(+0.014985062),FQ(+0.011518777),FQ(-0.008754448),
    FQ(-0.006548327),FQ(+0.004796811),FQ(+0.003420655),FQ(-0.002356083),FQ(-0.001549687),
    FQ(+0.000955507),FQ(+0.000533374),FQ(-0.000248008)
};
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
int coeffs[5] = {FQ(+0.399564099),FQ(+0.000000000),FQ(-0.399564099),FQ(-0.000000000),FQ(+0.598256395)};

bash-3.2$ python util_iir.py peaking 0.2 2.5 12.0       
int coeffs[5] = {FQ(+1.259455676),FQ(-0.564243794),FQ(+0.566475599),FQ(-0.564243794),FQ(+0.825931275)};
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

Example Application #1 - Cabsim
----------------------------------

Stereo Cabinet Simulation with Tone/Volume and USB Audio Mixing. See this video for a demonstration. The firmware is first
written to FLASH memory using the 'flexfx.py' script. After that the firmware reboots and enumerates as a USB audio device
resulting in audio. The first few chords are sent from the guitar ADC to the line out DAC unprocessed. The 'flexfx.py' script
is then used to load an IR file called 'ir1.wav'and then used to load another IR file called 'ir2.wav'.
The firmware is performing 25 msec of IR convolution (at a 48 kHz audio sample rate) on both left and right audio channels
using 32/64 bit fixed-point DSP.
https://raw.githubusercontent.com/markseel/flexfx_kit/master/app_cabsim.mp4

```C
#include "flexfx.h" // Defines config variables, I2C and GPIO functions, DSP functions, etc.
#include <math.h>   // Floating point for filter coeff calculations in the background process.
#include <string.h>

const char* product_name_string    = "Example"; // Your company/product name
const int   audio_sample_rate      = 48000;     // Audio sampling frequency
const int   usb_output_chan_count  = 2;         // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;         // 2 USB audio class 2.0 input channels
const int   i2s_channel_count      = 2;         // ADC/DAC channels per SDIN/SDOUT wire
const char  interface_text[]       = "No interface is specified";
const char  controller_app[]       = "No controller is available";

const int   i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

#define PROP_PRODUCT_ID      0x0101                      // Your product ID, must not be 0x0000!
#define PROP_EXAMPLE_PROPS   (PROP_PRODUCT_ID << 16)     // Base property ID value
#define PROP_EXAMPLE_VOL_MIX (PROP_EXAMPLE_PROPS+0x0001) // prop[1:5] = [volume,blend,0,0,0]
#define PROP_EXAMPLE_TONE    (PROP_EXAMPLE_PROPS+0x0002) // Coeffs, prop[1:5]=[B0,B1,B2,A1,A2]
#define PROP_EXAMPLE_IRDATA  (PROP_EXAMPLE_PROPS+0x8000) // 5 IR samples values per property

#define IR_PROP_ID(xx)  ((xx) & 0xFFFF8000) // IR property ID has two parts (ID and offset)
#define IR_PROP_IDX(xx) ((xx) & 0x00000FFF) // Up to 5*0xFFF samples (5 samples per property)

static void adc_read( double values[4] ) // I2C controlled 4-channel ADC (0.0 <= value < 1.0).
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

void app_control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
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

// Single-pole low pass filter implemented with a biquad IIR (only uses B0,B1,A1 coeffs).
int lopass_coeffs[5] = {FQ(1.0),0,0,0,0};
int lopass_stateL[4] = {0,0,0,0}, lopass_stateR[4] = {0,0,0,0}; // Initial filter state/history.

void app_mixer( const int usb_output[32], int usb_input[32],
                const int i2s_output[32], int i2s_input[32],
                const int dsp_output[32], int dsp_input[32], const int property[6] )
{
    static int volume = FQ(0.0), blend = FQ(0.5); // Initial volume and blend/mix levels.

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
    i2s_input[6] = dsp_multiply(blend,i2s_input[6])/2 + dsp_multiply(FQ(1)-blend,usb_output[0])/2;
    i2s_input[7] = dsp_multiply(blend,i2s_input[7])/2 + dsp_multiply(FQ(1)-blend,usb_output[1])/2;

    // Apply master volume to the I2S driver inputs (i.e. to the data going to the audio DACs).
    i2s_input[6] = dsp_multiply( i2s_input[6], volume );
    i2s_input[7] = dsp_multiply( i2s_input[7], volume );

    // Bypass DSP processing (pass ADC samples to the DAC) if a switch on I/O port #0 is pressed.
    if( port_read(1) == 0 ) { i2s_input[0] = i2s_output[0]; i2s_input[1] = i2s_output[1]; }
    // Light an LED attached to I/O port #2 if the bypass switch on I/O port #1 is pressed.
    port_write( 2, port_read(1) != 0 );
}

// Cabinet simulation impulse response (IR) data (e.g. filter coefficients) and filter state.
int ir_coeff[2400], ir_state[2400]; // DSP data *must* be non-static global!

void dsp_initialize( void ) // Called once upon boot-up.
{
    memset( &ir_coeff, 0, sizeof(ir_coeff) );
    memset( &ir_state, 0, sizeof(ir_state) );
    ir_coeff[0] = ir_coeff[1200] = FQ(+1.0);
}

void app_thread1( int samples[32], const int property[6] )
{
    // Check for properties containing new cabsim IR data, save new data to RAM
    if( IR_PROP_ID( property[0] ) == IR_PROP_ID( PROP_EXAMPLE_IRDATA ) &&
        IR_PROP_IDX( property[0] ) > 0 && IR_PROP_IDX( property[0] ) < 2400/5 )
    {
        int offset = 5 * IR_PROP_IDX( property[0] ); // Five samples per property
        ir_coeff[offset+0] = property[1] / 32; ir_coeff[offset+1] = property[2] / 32;
        ir_coeff[offset+2] = property[3] / 32; ir_coeff[offset+3] = property[4] / 32;
        ir_coeff[offset+4] = property[5] / 32;
    }
    samples[2] = 0; samples[3] = 1 << 31; // Initial 64-bit Q1.63 accumulator value
    samples[4] = 0; samples[5] = 1 << 31; // Initial 64-bit Q1.63 accumulator value
    // Perform 240-sample convolution (1st 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*0, ir_state+240*0, samples+2, samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*5, ir_state+240*5, samples+4, samples+5 );
}

void app_thread2( int samples[32], const int property[6] )
{
    // Perform 240-sample convolution (2nd 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*1, ir_state+240*1, samples+2, samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*6, ir_state+240*6, samples+4, samples+5 );
}

void app_thread3( int samples[32], const int property[6] )
{
    // Perform 240-sample convolution (3rd 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*2, ir_state+240*2, samples+2, samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*7, ir_state+240*7, samples+4, samples+5 );
}

void app_thread4( int samples[32], const int property[6] )
{
    // Perform 240-sample convolution (4th 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*3, ir_state+240*3, samples+2, samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*8, ir_state+240*8, samples+4, samples+5 );
}

void app_thread5( int samples[32], const int property[6] )
{
    static bool muted = 0;
    // Check IR property -- Mute at start of new IR loading, un-mute when done.
    if( IR_PROP_ID( property[0] ) == IR_PROP_ID( PROP_EXAMPLE_IRDATA ) )
    {
        if( IR_PROP_IDX( property[0] ) == 0 )        muted = 1; // First IR property -- Mute
        if( IR_PROP_IDX( property[0] ) == 2400/5-1 ) muted = 0; // Last IR property -- Un-Mute
    }
    // Perform 240-sample convolution (5th and last 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*4, ir_state+240*4, samples+2, samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*9, ir_state+240*9, samples+4, samples+5 );
    // Extract 32-bit Q28 from 64-bit Q63 and then apply mute/un-mute based on IR loading activity.
    DSP_EXT( samples[0], samples[2], samples[3] ); samples[0] = muted ? 0 : samples[0];
    DSP_EXT( samples[1], samples[4], samples[5] ); samples[1] = muted ? 0 : samples[1];
}
```

Example Application #2 - Chorus
----------------------------------

Chorus example with two voices showing how to create LFO's and how to use 2nd order interpolation. See this video for a demonstration of building and loading the firmware exmaple, and the chorus audio effect.
https://raw.githubusercontent.com/markseel/flexfx_kit/master/app_chorus.mp4

```C
#include "flexfx.h" // Defines config variables, I2C and GPIO functions, DSP functions, etc.
#include <math.h>   // Floating point for filter coeff calculations in the background process.
#include <string.h>

const char* product_name_string    = "Example"; // Your company/product name
const int   audio_sample_rate      = 48000;     // Audio sampling frequency
const int   usb_output_chan_count  = 2;         // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;         // 2 USB audio class 2.0 input channels
const int   i2s_channel_count      = 2;         // ADC/DAC channels per SDIN/SDOUT wire
const char  interface_text[]       = "No interface is specified";
const char  controller_app[]       = "No controller is available";

const int   i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

void app_control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] ) {}

void app_mixer( const int usb_output[32], int usb_input[32],
                const int i2s_output[32], int i2s_input[32],
                const int dsp_output[32], int dsp_input[32], const int property[6] )
{
    dsp_input[0] = i2s_output[6]/2 - i2s_output[7]/2; // DSP left input channel = guitar input.
    dsp_input[1] = i2s_output[6]/2 - i2s_output[7]/2; // DSP right input channel = guitar input.
    i2s_input[6] = dsp_output[0]; // Line out left channel = DSP output left channel
    i2s_input[7] = dsp_output[1]; // Line out right channel = DSP output right channel
}

void app_thread1( int samples[32], const int property[6] )
{
    // Define LFO frequencies
    static int delta1 = FQ(1.7/48000.0); // LFO frequency 1.7 Hz @ 48 kHz
    static int delta2 = FQ(2.3/48000.0); // LFO frequency 2.3 Hz @ 48 kHz
    // Update LFO time: Increment each and limit to 1.0 -- wrap as needed.
    static int time1 = FQ(0.0); time1 += delta1; if(time1 > FQ(1.0)) time1 -= FQ(1.0);
    static int time2 = FQ(0.0); time2 += delta2; if(time2 > FQ(1.0)) time2 -= FQ(1.0);
    // II is index into sine table (0.0 < II < 1.0), FF is the fractional remainder
    // Use 2nd order interpolation to smooth out lookup values.
    // Index and fraction portion of Q28 sample value: snnniiii,iiiiiiff,ffffffff,ffffffff
    int ii, ff;
    ii = (time1 & 0x0FFFFFFF) >> 18, ff = (time1 & 0x0003FFFF) << 10;
    samples[2] = dsp_lagrange( ff, dsp_sine_lut[ii+0], dsp_sine_lut[ii+1], dsp_sine_lut[ii+2] );
    ii = (time2 & 0x0FFFFFFF) >> 18, ff = (time2 & 0x0003FFFF) << 10;
    samples[3] = dsp_lagrange( ff, dsp_sine_lut[ii+0], dsp_sine_lut[ii+1], dsp_sine_lut[ii+2] );
    // Send LFO values downstream for use by other DSP threads, ensure that they're less than +1.0.
    samples[2] = dsp_multiply( samples[2], FQ(0.999) ); // LFO #1 stored in sample 2 for later use.
    samples[3] = dsp_multiply( samples[3], FQ(0.999) ); // LFO #2 stored in sample 3 for later use.
}

void app_thread2( int samples[32], const int property[6] )
{
    // --- Generate wet signal #1 using LFO #1
    static int delay_fifo[1024], delay_index = 0; // Chorus delay line
    static int depth = FQ(+0.10);
    // Scale lfo by chorus depth and convert from [-1.0 < lfo < +1.0] to [+0.0 < lfo < +1.0].
    int lfo = (dsp_multiply( samples[2], depth ) / 2) + FQ(0.4999);
    // Index and fraction portion of Q28 LFO value: snnniiii,iiiiiiff,ffffffff,ffffffff
    int ii = (lfo & 0x0FFFFFFF) >> 18, ff = (lfo & 0x0003FFFF) << 10;
    delay_fifo[delay_index-- & 1023] = samples[0]; // Update the sample delay line.
    // Get samples from delay -- handle wrapping of index values.
    int i1 = (delay_index+ii+0)&1023, i2 = (delay_index+ii+1)&1023, i3 = (delay_index+ii+2)&1023;
    // Interpolate and store wet signal #1 for use in another DSP thread below.
    samples[2] = dsp_lagrange( ff, delay_fifo[i1], delay_fifo[i2], delay_fifo[i3] );
}

void app_thread3( int samples[32], const int property[6] )
{
    // --- Generate wet signal #2 using LFO #2
    static int delay_fifo[1024], delay_index = 0; // Chorus delay line
    static int depth = FQ(+0.05);
    // Scale lfo by chorus depth and convert from [-1.0 < lfo < +1.0] to [+0.0 < lfo < +1.0].
    int lfo = (dsp_multiply( samples[3], depth ) / 2) + FQ(0.4999);
    // Index and fraction portion of Q28 LFO value: snnniiii,iiiiiiff,ffffffff,ffffffff
    int ii = (lfo & 0x0FFFFFFF) >> 18, ff = (lfo & 0x0003FFFF) << 10;
    delay_fifo[delay_index-- & 1023] = samples[0]; // Update the sample delay line.
    // Get samples from delay -- handle wrapping of index values.
    int i1 = (delay_index+ii+0)&1023, i2 = (delay_index+ii+1)&1023, i3 = (delay_index+ii+2)&1023;
    // Interpolate and store wet signal #1 for use in another DSP thread below.
    samples[3] = dsp_lagrange( ff, delay_fifo[i1], delay_fifo[i2], delay_fifo[i3] );
}

void app_thread4( int samples[32], const int property[6] )
{
    int blend1 = FQ(+0.50), blend2 = FQ(+0.30);;
    // Mix dry signal with wet #1 and wet #2 and send to both left and right channels (0 and 1).
    samples[2] = dsp_multiply(samples[0],FQ(1.0)-blend1)/2 + dsp_multiply(samples[2],blend1)/2;
    samples[3] = dsp_multiply(samples[0],FQ(1.0)-blend2)/2 + dsp_multiply(samples[3],blend2)/2;
    samples[0] = samples[1] = samples[2]/2 + samples[3]/2;
}

void app_thread5( int samples[32], const int property[6] )
{
}
```
Example Application #3 - Overdrive
----------------------------------

Overdrive example demonstrating up/down sampling, anti-aliasing filters, and the use of look-up tables and Lagrange interpolation to 
create a simple preamp model. Up/down sampling by a factor of 2 brings the internal sampling rate to 384 kHz to help manage the 
aliasing of harmonics created from the nonlinear behavior of the preamp.

```C
#include "flexfx.h"
#include <math.h>
#include <string.h>

const char* product_name_string    = "Example"; // Your company/product name
const int   audio_sample_rate      = 192000;    // Audio sampling frequency
const int   usb_output_chan_count  = 2;         // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;         // 2 USB audio class 2.0 input channels
const int   i2s_channel_count      = 2;         // ADC/DAC channels per SDIN/SDOUT wire
const char  interface_text[]       = "No interface is specified";
const char  controller_app[]       = "No controller is available";

const int   i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

void app_control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] ) {}

void app_mixer( const int usb_output[32], int usb_input[32],
                const int i2s_output[32], int i2s_input[32],
                const int dsp_output[32], int dsp_input[32], const int property[6] )
{
    // Convert the two ADC inputs into a single pseudo-differential mono input (mono = L - R).
    int guitar_in = i2s_output[6] - i2s_output[7];

    // Route instrument input to the USB input and to the DSP input.
    usb_input[0] = usb_input[1] = guitar_in;
    dsp_input[0] = dsp_input[1] = guitar_in / 8; // Convert from Q31 to Q28

    // Reduce DSP result to avoid clipping, convert it from Q28 to Q31, send it to the audio DAC.
    i2s_input[6] = i2s_input[7] = 8 * dsp_multiply( dsp_output[0], FQ(+0.8) );
}

// util_fir.py 0.001 0.125 1.0 -100
int antialias_state1[64], antialias_state2[64], antialias_coeff[64] =
{
    FQ(-0.000000077),FQ(-0.000001205),FQ(-0.000005357),FQ(-0.000014901),FQ(-0.000030776),
    FQ(-0.000049283),FQ(-0.000058334),FQ(-0.000035116),FQ(+0.000052091),FQ(+0.000235288),
    FQ(+0.000530261),FQ(+0.000914292),FQ(+0.001304232),FQ(+0.001544675),FQ(+0.001417440),
    FQ(+0.000681095),FQ(-0.000857660),FQ(-0.003249472),FQ(-0.006305108),FQ(-0.009524756),
    FQ(-0.012085259),FQ(-0.012909300),FQ(-0.010823540),FQ(-0.004789122),FQ(+0.005836344),
    FQ(+0.021063799),FQ(+0.040127668),FQ(+0.061481322),FQ(+0.082955733),FQ(+0.102066436),
    FQ(+0.116416104),FQ(+0.124112485),FQ(+0.124112485),FQ(+0.116416104),FQ(+0.102066436),
    FQ(+0.082955733),FQ(+0.061481322),FQ(+0.040127668),FQ(+0.021063799),FQ(+0.005836344),
    FQ(-0.004789122),FQ(-0.010823540),FQ(-0.012909300),FQ(-0.012085259),FQ(-0.009524756),
    FQ(-0.006305108),FQ(-0.003249472),FQ(-0.000857660),FQ(+0.000681095),FQ(+0.001417440),
    FQ(+0.001544675),FQ(+0.001304232),FQ(+0.000914292),FQ(+0.000530261),FQ(+0.000235288),
    FQ(+0.000052091),FQ(-0.000035116),FQ(-0.000058334),FQ(-0.000049283),FQ(-0.000030776),
    FQ(-0.000014901),FQ(-0.000005357),FQ(-0.000001205),FQ(-0.000000077)
};

// util_iir.py highpass 0.0003 0.707 0, util_iir.py peaking 0.001 0.707 +6.0
int emphasis1_state[8] = {0,0,0,0,0,0,0,0}, emphasis1_coeff[10] =
{
    FQ(+0.998667822),FQ(-1.997335644),FQ(+0.998667822),FQ(+1.997333870),FQ(-0.997337419),
    FQ(+1.003121053),FQ(-1.993688826),FQ(+0.990607128),FQ(+1.993688826),FQ(-0.993728181),
};
// util_iir.py highpass 0.0002 0.707 0, util_iir.py peaking 0.002 0.707 +6.0
int emphasis2_state[8] = {0,0,0,0,0,0,0,0}, emphasis2_coeff[10] =
{
    FQ(+0.999111684),FQ(-1.998223368),FQ(+0.999111684),FQ(+1.998222579),FQ(-0.998224157),
    FQ(+1.006222470),FQ(-1.987338895),FQ(+0.981273349),FQ(+1.987338895),FQ(-0.987495819),
};
// util_iir.py highpass 0.0001 0.707 0, util_iir.py lowpass 0.001 0.707 -3.0
int emphasis3_state[8] = {0,0,0,0,0,0,0,0}, emphasis3_coeff[10] =
{
    FQ(+0.999555743),FQ(-1.999111487),FQ(+0.999555743),FQ(+1.999111289),FQ(-0.999111684),
    FQ(+0.996194429),FQ(-1.973695761),FQ(+0.977744852),FQ(+1.973695761),FQ(-0.973939281),
};

// util_iir.py lowpass 0.09 0.707 0
int lowpass1_state[4] = {0,0,0,0}, lowpass1_coeff[5] =
{
    FQ(+0.056446120),FQ(+0.112892239),FQ(+0.056446120),FQ(+1.224600759),FQ(-0.450385238),
};
// util_iir.py lowpass 0.03 0.707 0
int lowpass2_state[4] = {0,0,0,0}, lowpass2_coeff[5] =
{
    FQ(+0.007820070),FQ(+0.015640140),FQ(+0.007820070),FQ(+1.734695116),FQ(-0.765975395),
};
// util_iir.py lowpass 0.01 0.707 0
int lowpass3_state[4] = {0,0,0,0}, lowpass3_coeff[5] =
{
    FQ(+0.000944686),FQ(+0.001889372),FQ(+0.000944686),FQ(+1.911184796),FQ(-0.914963539),
};

// Simple preamp model (-1.0 <= output < +1.0)
// Apply gain factor. Lookup the preamp transfer function and interpolate to smooth out the lookup
// result. Apply slew-rate limiting to the output.

int preamp1[3], preamp2[3], preamp3[3];

int preamp_model( int xx, int gain, int bias, int slewlim, int* state )
{
    // Apply gain (total preamp gain = 8 * gain)
    xx = dsp_multiply( xx, gain );
    // Table lookup
    if( xx >= 0 ) {
        int ii = (xx & 0xFFFFC000) >> 14, ff = (xx & 0x00003FFF) << 14;
        if( ii > 16383 ) ii = 16383;
        xx = dsp_lagrange( ff, dsp_tanh_14[ii+0], dsp_tanh_14[ii+1], dsp_tanh_14[ii+2] );
    } else {
        int ii = (-xx & 0xFFFFC000) >> 14, ff = (-xx & 0x00003FFF) << 14;
        if( ii > 16383 ) ii = 16383;
        xx = -dsp_lagrange( ff, dsp_tanh_14[ii+0], dsp_tanh_14[ii+1], dsp_tanh_14[ii+2] );
    }
    // Slew rate limit
    if( xx > state[2] + slewlim ) { xx = state[2] + slewlim; state[2] = xx; }
    if( xx < state[2] - slewlim ) { xx = state[2] - slewlim; state[2] = xx; }
    return xx;
}

void app_thread1( int samples[32], const int property[6] ) // Upsample
{
    // Up-sample by 2x by inserting zeros then apply the anti-aliasing filter
    samples[0] = 4 * dsp_fir_filt( samples[0], antialias_coeff, antialias_state1, 64 );
    samples[1] = 4 * dsp_fir_filt( 0,          antialias_coeff, antialias_state1, 64 );
}

void app_thread2( int samples[32], const int property[6] ) // Preamp stage 1
{
    // Perform stage 1 overdrive on the two up-sampled samples for the left channel.
    samples[0] = dsp_iir_filt( samples[0], emphasis1_coeff, emphasis1_state, 2 );
    samples[0] = preamp_model( samples[0], FQ(1.0), FQ(0.0), FQ(0.40), preamp1 );
    samples[0] = dsp_iir_filt( samples[0], lowpass1_coeff, lowpass1_state, 1 );
    samples[1] = dsp_iir_filt( samples[1], emphasis1_coeff, emphasis1_state, 2 );
    samples[1] = preamp_model( samples[1], FQ(1.0), FQ(0.0), FQ(0.40), preamp1 );
    samples[1] = dsp_iir_filt( samples[1], lowpass1_coeff, lowpass1_state, 1 );
}

void app_thread3( int samples[32], const int property[6] ) // Preamp stage 2
{
    // Perform stage 2 overdrive on the two up-sampled samples for the left channel.
    samples[0] = dsp_iir_filt( samples[0], emphasis2_coeff, emphasis2_state, 2 );
    samples[0] = preamp_model( samples[0], FQ(1.0), FQ(0.0), FQ(0.20), preamp2 );
    samples[0] = dsp_iir_filt( samples[0], lowpass2_coeff, lowpass2_state, 1 );
    samples[1] = dsp_iir_filt( samples[1], emphasis2_coeff, emphasis2_state, 2 );
    samples[1] = preamp_model( samples[1], FQ(1.0), FQ(0.0), FQ(0.20), preamp2 );
    samples[1] = dsp_iir_filt( samples[1], lowpass2_coeff, lowpass2_state, 1 );
}

void app_thread4( int samples[32], const int property[6] ) // Preamp stage 3
{
    // Perform stage 3 overdrive on the two up-sampled samples for the left channel.
    samples[0] = dsp_iir_filt( samples[0], emphasis3_coeff, emphasis3_state, 2 );
    samples[0] = preamp_model( samples[0], FQ(1.0), FQ(0.0), FQ(0.05), preamp3 );
    samples[0] = dsp_iir_filt( samples[0], lowpass3_coeff, lowpass3_state, 1 );
    samples[1] = dsp_iir_filt( samples[1], emphasis3_coeff, emphasis3_state, 2 );
    samples[1] = preamp_model( samples[1], FQ(1.0), FQ(0.0), FQ(0.05), preamp3 );
    samples[1] = dsp_iir_filt( samples[1], lowpass3_coeff, lowpass3_state, 1 );
}

void app_thread5( int samples[32], const int property[6] ) // Downsample
{
    // Down-sample by 2x by band-limiting via anti-aliasing filter and then discarding 1 sample.
    samples[0] = dsp_fir_filt( samples[0], antialias_coeff, antialias_state2, 64 );
                 dsp_fir_filt( samples[1], antialias_coeff, antialias_state2, 64 );
}
```
Example Application #4 - Reverb
----------------------------------

Reverb example implemting a port of the FreeVerb algorithm based on the Schroeder-Moorer reverberation model.  A nice improvement would be to use a potentiometer (sensed via an ADC attached via I2C in the 'control' function) to select the reverb type.  The potentiometer code would perhaps generate a value of 1 through 9 depending on the pot's rotation setting.  The value would then be used to select the reverb's wet/dry mix, stereo width, room size, and room reflection/damping.  Note that there's plenty of processing power left over to implement other algorithms along with this reverb such as graphic EQ's, flanger/chorus, etc.

```C
#include "flexfx.h"
#include <math.h>
#include <string.h>

const char* product_name_string   = "Example"; // Your company/product name
const int   audio_sample_rate     = 48000;     // Audio sampling frequency
const int   usb_output_chan_count = 2;         // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count  = 2;         // 2 USB audio class 2.0 input channels
const int   i2s_channel_count     = 2;         // ADC/DAC channels per SDIN/SDOUT wire
const char  interface_string[]    = "No interface is specified";
const char  controller_string[]   = "No controller is available";

const int   i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

void app_control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] ) {}

void app_mixer( const int usb_output[32], int usb_input[32],
                const int i2s_output[32], int i2s_input[32],
                const int dsp_output[32], int dsp_input[32], const int property[6] )
{
    // Convert the two ADC inputs into a single pseudo-differential mono input (mono = L - R).
    int guitar_in = i2s_output[0] - i2s_output[1];
    // Route instrument input to the left USB input and to the DSP input.
    dsp_input[0] = (usb_input[0] = guitar_in) / 8; // DSP samples need to be Q28 formatted.
    // Route DSP result to the right USB input and the audio DAC.
    usb_input[1] = i2s_input[0] = i2s_input[1] = dsp_output[0] * 8; // Q28 to Q31
}

int _comb_bufferL   [8][2048], _comb_bufferR   [8][2048]; // Delay lines for comb filters
int _comb_stateL    [8],       _comb_stateR    [8];       // Comb filter state (previous value)
int _allpass_bufferL[4][1024], _allpass_bufferR[4][1024]; // Delay lines for allpass filters

int _allpass_feedbk    = FQ(0.5); // Reflection decay/dispersion
int _stereo_spread     = 23;      // Buffer index spread for stereo separation
int _comb_delays   [8] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // From FreeVerb
int _allpass_delays[8] = { 556, 441, 341, 225 }; // From FreeVerb

int _wet_dry_blend  = FQ(0.2); // Parameter: Wet/dry mix setting (0.0=dry)
int _stereo_width   = FQ(0.2); // Parameter: Stereo width setting
int _comb_damping   = FQ(0.2); // Parameter: Reflection damping factor (aka 'reflectivity')
int _comb_feedbk    = FQ(0.2); // Parameter: Reflection feedback ratio (aka 'room size')

void dsp_initialize( void ) // Called once upon boot-up.
{
    memset( _comb_bufferL, 0, sizeof(_comb_bufferL) );
    memset( _comb_stateL,  0, sizeof(_comb_stateL) );
    memset( _comb_bufferR, 0, sizeof(_comb_bufferR) );
    memset( _comb_stateR,  0, sizeof(_comb_stateR) );
}

inline int _comb_filterL( int xx, int ii, int nn ) // yy[k] = xx[k] + g1*xx[k-M1] - g2*yy[k-M2]
{
    ii = (_comb_delays[nn] + ii) & 2047; // Index into sample delay FIFO
    int yy = _comb_bufferL[nn][ii];
    _comb_stateL[nn] = dsp_multiply( yy, FQ(1.0) - _comb_damping )
                     + dsp_multiply( _comb_stateL[nn], _comb_damping );
    _comb_bufferL[nn][ii] = xx + dsp_multiply( _comb_stateL[nn], _comb_feedbk );
    return yy;
}

inline int _comb_filterR( int xx, int ii, int nn ) // yy[k] = xx[k] + g1*xx[k-M1] - g2*yy[k-M2]
{
    ii = (_comb_delays[nn] + ii + _stereo_spread) & 2047; // Index into sample delay FIFO
    int yy = _comb_bufferR[nn][ii];
    _comb_stateR[nn] = dsp_multiply( yy, FQ(1.0) - _comb_damping )
                     + dsp_multiply( _comb_stateR[nn], _comb_damping );
    _comb_bufferR[nn][ii] = xx + dsp_multiply( _comb_stateR[nn], _comb_feedbk );
    return yy;
}

inline int _allpass_filterL( int xx, int ii, int nn ) // yy[k] = xx[k] + g * xx[k-M] - g * xx[k]
{
    ii = (_allpass_delays[nn] + ii) & 1023; // Index into sample delay FIFO
    int yy = _allpass_bufferL[nn][ii] - xx;
    _allpass_bufferL[nn][ii] = xx + dsp_multiply( _allpass_bufferL[nn][ii], _allpass_feedbk );
    return yy;
}

inline int _allpass_filterR( int xx, int ii, int nn ) // yy[k] = xx[k] + g * xx[k-M] - g * xx[k]
{
    ii = (_allpass_delays[nn] + ii + _stereo_spread) & 1023; // Index into sample delay FIFO
    int yy = _allpass_bufferR[nn][ii] - xx;
    _allpass_bufferR[nn][ii] = xx + dsp_multiply( _allpass_bufferR[nn][ii], _allpass_feedbk );
    return yy;
}

void app_thread1( int samples[32], const int property[6] )
{
    // ----- Left channel reverb
    static int index = 0; ++index; // Used to index into the sample FIFO delay buffer
    // Eight parallel comb filters ...
    samples[2] = _comb_filterL( samples[0]/8, index, 0 ) + _comb_filterL( samples[0]/8, index, 1 )
               + _comb_filterL( samples[0]/8, index, 2 ) + _comb_filterL( samples[0]/8, index, 3 )
               + _comb_filterL( samples[0]/8, index, 4 ) + _comb_filterL( samples[0]/8, index, 5 )
               + _comb_filterL( samples[0]/8, index, 6 ) + _comb_filterL( samples[0]/8, index, 7 );
    // Four series all-pass filters ...
    samples[2] = _allpass_filterL( samples[2], index, 0 );
    samples[2] = _allpass_filterL( samples[2], index, 1 );
    samples[2] = _allpass_filterL( samples[2], index, 2 );
    samples[2] = _allpass_filterL( samples[2], index, 3 );
}

void app_thread2( int samples[32], const int property[6] )
{
    // ----- Right channel reverb
    static int index = 0; ++index; // Used to index into the sample FIFO delay buffer
    // Eight parallel comb filters ...
    samples[1] = _comb_filterR( samples[0]/8, index, 0 ) + _comb_filterR( samples[0]/8, index, 1 )
               + _comb_filterR( samples[0]/8, index, 2 ) + _comb_filterR( samples[0]/8, index, 3 )
               + _comb_filterR( samples[0]/8, index, 4 ) + _comb_filterR( samples[0]/8, index, 5 )
               + _comb_filterR( samples[0]/8, index, 6 ) + _comb_filterR( samples[0]/8, index, 7 );
    // Four series all-pass filters ...
    samples[3] = _allpass_filterR( samples[3], index, 0 );
    samples[3] = _allpass_filterR( samples[3], index, 1 );
    samples[3] = _allpass_filterR( samples[3], index, 2 );
    samples[3] = _allpass_filterR( samples[3], index, 3 );
}

void app_thread3( int samples[32], const int property[6] )
{
    // Final mixing and stereo synthesis
    int dry = _wet_dry_blend, wet = FQ(1.0) - _wet_dry_blend;
    // Coefficients for stereo separation
    int wet1 = _stereo_width / 2 + 0.5;
    int wet2 = (FQ(1.0) - _stereo_width) / 2;
    // Final mixing and stereo separation for left channel
    samples[0] = dsp_multiply( dry, samples[0] )
               + dsp_multiply( wet, dsp_multiply( samples[2], wet1 ) +
                                    dsp_multiply( samples[3], wet2 ) );
    // Final mixing and stereo separation for right channel
    samples[1] = dsp_multiply( dry, samples[1] )
               + dsp_multiply( wet, dsp_multiply( samples[2], wet2 ) +
                                    dsp_multiply( samples[3], wet1 ) );
}

void app_thread4( int samples[32], const int property[6] )
{
}

void app_thread5( int samples[32], const int property[6] )
{
}
```

Prebuilt Effects
----------------------------------
The FlexFX kit contains some highly optimized effects. These effects are in binary form and can be used for free on FlexFX boards. Here's an example of how to use the optimzied stereo Cabinet simulator that supports 37.5 msec of IR processing in stereo mode at 48 kHz, and 75 msec of IR processing in mono mode at 48 kHz.

The FlexFX properties definitions for uploading IR data (stored in wave files on a USB atteched host computer) is documented in 'efx_cabsim.txt' and is also available via USB MIDI by issueing the FlexFX USB/MIDI property for returning a device's MIDI interface (the text is returned via USB).  

This effect also supports the HTML5 interface for controlling the device (firmware upgrading, uploading IR data, etc). The javascript code for the effect is returned via USB MIDI by issueing the FlexFX USB/MIDI property for returning a device's javascript controller interface.  The HTML5 application called 'flexfx.html' does this automatically and will displayt this device's GUI interface if the device is pluuged into the host computer via a USB cable. Google Chrome must be used.

```
#include "efx_cabsim.h"

const char* product_name_string = "FlexFX Cabsim";  // Your company/product name

void app_control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    efx_cabsim__app_control( rcv_prop, usb_prop, dsp_prop );
}

void app_mixer( const int usb_output[32], int usb_input[32],
                const int i2s_output[32], int i2s_input[32],
                const int dsp_output[32], int dsp_input[32], const int property[6] )
{
    efx_cabsim__app_mixer( usb_output, usb_input, i2s_output, i2s_input,
                           dsp_output, dsp_input, property );
}

void app_thread1( int samples[32], const int property[6] ) {efx_cabsim__app_thread1(samples,property);}
void app_thread2( int samples[32], const int property[6] ) {efx_cabsim__app_thread2(samples,property);}
void app_thread3( int samples[32], const int property[6] ) {efx_cabsim__app_thread3(samples,property);}
void app_thread4( int samples[32], const int property[6] ) {efx_cabsim__app_thread4(samples,property);}
void app_thread5( int samples[32], const int property[6] ) {efx_cabsim__app_thread5(samples,property);}
```

Discovery and Control
----------------------------
All USB MIDI data flow between a host computer and FlexFX devices occurs via FlexFX properties (see 'Run-time' Control above). FlexFX devices, including the optimized prebuilt effects (see 'Prebuilt Effects' above), support a discovery process whereby any host computer can querry a FlexFX device for both its MIDI interface and its Javascript control code via a FlexFX property.

The FlexFx property interface data, returned in a human readable text) is returned via USB MIDI if the device receives FlexFX properties with ID's of 0x21 (begin), 0x22 (next), and 0x23 (end). The returned text can be used to provide additonal FlexFX property definitions for device specific (or effect specific) control. This textual data is automatically included in the FlexFX firmware image (see 'Development Steps' above) if a .txt file with the same name as the effect being built exists (e.g. efx_cabsim.txt for efx_cabsim.c firmware).

Here's an example of a textual property interface definition for the 'efx_cabsim' effect. Note that this exact text
is returned upon issuing a FlexFX proeprty with ID = 0x0004 to the target FlexFX device.

```
# FLEXFX STEREO CABSIM Property Interface
#
# Stereo Cabinet Simulator using impulse responses. Impulse responses to upload
# must be stored in a wave file (RIFF/WAV format) and have a sampling frequency
# of 48 kHz. Both mono and stereo source data is supported.  Stereo can also be
# employed by specifying two mono WAV files.
#
# PROP ID   DIRECTION        DESCRIPTION
# 0001      Bidirectional    Return 3DEGFLEX, versions and props[4:5] to host
# 0002      Bidirectional    Return the device product name (up to 40 bytes)
# 0003      Device to Host   Start dumping the text in this file
# 0004      Bidirectional    Next 40 bytes of property interface text
# 0005      Host to Device   End of property interface text dump
# 0006      Device to Host   Start a controller javascript code dump
# 0007      Bidirectional    Next 40 bytes of javascript code text
# 0008      Host to Device   End of controller javascript code text
# 0009      Bidirectional    Begin firmware upgrade, echoed back to host
# 000A      Bidirectional    Next 32 bytes of firmware image data, echoed
# 000B      Bidirectional    End firmware upgrade, echoed back to host
# 000C      Bidirectional    Begin flash user data load, echoed back to host
# 000D      Bidirectional    Next 32 bytes of flash user data, echoed
# 000E      Bidirectional    End flash data loading, echoed back to host
# 000F      Bidirectional    Query/return DSP thread run-time (audio cycles)
# PROP ID   DIRECTION        DESCRIPTION
# 1000      Host to Device   Return volume,tone,preset control settings
# 1001      Bidirectional    Update controls (overrides physical controls)
# 1n01      Bidirectional    Up to 20 charactr name for preset N (1<=N<=9)
# 1n02      Bidirectional    Begin data upload for preset N, begin upload ACK
# 1n03      Bidirectional    Five IR data words for preset N or echoed data
# 1n04      Bidirectional    End data upload for preset N or end upload ACK
# 1n05      Bidirectional    First 20 chars of data file name for preset N
# 1n06      Bidirectional    Next 20 chars of data file name for preset N
# 1n07      Bidirectional    Last 20 chars of data file name for preset N
#
# Property layout for control (knobs, pushbuttons, etc) Values shown are 32-bit
# values represented in ASCII/HEX format or as floating point values ranging
# from +0.0 up to (not including) +1.0.
#
# +------- Effect parameter identifier (Property ID)
# |
# |    +-------------------------------- Volume level
# |    |     +-------------------------- Tone setting
# |    |     |     +-------------------- Reserved
# |    |     |     |     +-------------- Reserved
# |    |     |     |     |     +-------- Preset selection (1 through 9)
# |    |     |     |     |     |+------- Enabled (1=yes,0=bypassed)
# |    |     |     |     |     ||+------ InputL  (1=plugged,0=unplugged)
# |    |     |     |     |     |||+----- OutputL (1=plugged,0=unplugged)
# |    |     |     |     |     ||||+---- InputR  (1=plugged,0=unplugged)
# |    |     |     |     |     |||||+--- OutputR (1=plugged,0=unplugged)
# |    |     |     |     |     ||||||+-- Expression (1=plugged,0=unplugged)
# |    |     |     |     |     |||||||+- USB Audio (1=active)
# |    |     |     |     |     ||||||||
# 1001 0.500 0.500 0.500 0.500 91111111
#
# Property layout for preset data loading (loading IR data). Values shown are
# 32-bit values represented in ASCII/HEX format.
#
# +---------- Effect parameter identifier (Property ID)
# |
# |+--- Preset number (1 through 9)
# ||
# 1n02 0 0 0 0 0 # Begin IR data loading for preset N
# 1n03 A B C D E # Five of the next IR data words to load into preset N
# 1n04 0 0 0 0 0 # End IR data loading for preset N
```

The FlexFX HTML5 control javascript source code is returned via USB MIDI if the device receives FlexFX properties with ID's of 0x31 (begin), 0x32 (next), and 0x33 (end). The returned code can be used in HTML5 applications to access and control FlexFX devices via HTML MIDI whoch is supported by Google Chrome. The HTML5 application called 'flexfx.html' will sense USB MIDI events, including the plugging and unplugging of FlexFX devices, querry the device for its javascript controller code, and display the device's GUI interface on a webpage. This javascript codeis automatically included in the FlexFX firmware image (see 'Development Steps' above) if a .js file with the same name as the effect being built exists (e.g. efx_cabsim.js for efx_cabsim.c firmware).

Here's an example of HTML5 controller for the 'efx_cabsim' effect:
![alt tag](https://raw.githubusercontent.com/markseel/flexfx_kit/master/efx_cabsim.png)
