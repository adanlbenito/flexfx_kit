#include "flexfx.h"      // Defines config variables, I2C and GPIO functions, etc.
#include <math.h>        // Floating point for filter coeff calculations in the background process.
#include <string.h>

// System configuration ...

const char* company_name_string    = "FlexFX";  // Your company name
const char* product_name_string    = "Example"; // Your product name
const int   audio_sample_rate      = 48000;     // Audio sampling frequency
const int   usb_output_chan_count  = 2;         // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;         // 2 USB audio class 2.0 input channels
const int   i2s_channel_count      = 2;         // ADC/DAC channels per SDIN/SDOUT wire

const int   i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

// The control task is called at a rate of 1000 Hz and should be used to implement audio CODEC
// initialization/control, pot and switch sensing via I2C ADC's, handling of properties from USB
// MIDI, and generation of properties to be consumed by the USB MIDI host and by the DSP threads.
// The incoming USB property 'rcv_prop' is valid if its ID is non-zero. Outgoing USB and DSP props
// will be sent out if their ID is non-zero - but they are only available for routing if their ID's
// are zero (non-zero indicating that routing is still in progress). It's OK to use floating point
// calculations here since this thread is not a real-time audio or DSP thread.

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || usb_prop[0] != 0 ) return;
}

// The mixer function is called once per audio sample and is used to route USB, I2S and DSP samples.
// This function should only be used to route samples and for very basic DSP processing - not for
// substantial sample processing since this may starve the I2S audio driver. Do not use floating
// point operations since this is a real-time audio thread - all DSP operations and calculations
// should be performed using fixed-point math.
// NOTE: IIR, FIR, and BiQuad coeff and state data *must* be declared non-static global!

void mixer( const int* usb_output, int* usb_input,
            const int* i2s_output, int* i2s_input,
            const int* dsp_output, int* dsp_input, const int* property )
{
    usb_input[0]=usb_input[1] = i2s_output[6]-i2s_output[7]; // Send pseudo-diff ADC sample to USB.
}

// Audio Processing Threads. These functions run on tile 1 and are called once for each audio sample
// cycle. They cannot share data with the controller task or the mixer functions above that run on
// tile 0. The number of incoming and outgoing samples in the 'samples' array is set by the constant
// 'dsp_chan_count' defined above. Do not use floating point operations since these are real-time
// audio threads - all DSP operations and calculations should be performed using fixed-point math.
// NOTE: IIR, FIR, and BiQuad coeff and state data *must* be declared non-static global!

// Initialize DSP thread data (filter data and other algorithm data) here.
void dsp_initialize( void ) // Called once upon boot-up.
{
}

// Process samples (from the mixer function) and properties. Send results to stage 2.
void dsp_thread1( int* samples, const int* property )
{
}

// Process samples (from stage 1) and properties. Send results to stage 3.
void dsp_thread2( int* samples, const int* property )
{
}

// Process samples (from stage 2) and properties. Send results to stage 4.
void dsp_thread3( int* samples, const int* property )
{
}

// Process samples (from stage 3) and properties. Send results to stage 5.
void dsp_thread4( int* samples, const int* property )
{
}

// Process samples (from stage 4) and properties. Send results to the mixer function.
void dsp_thread5( int* samples, const int* property )
{
}
