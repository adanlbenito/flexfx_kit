#include "flexfx.h" // Defines config variables, I2C and GPIO functions, etc.
#include <math.h>   // Floating point for filter coeff calculations in the background process.
#include <string.h> // Memory and string functions

// System configuration ...

const char* product_name_string   = "FlexFX Example";   // Your product name
const char* usb_audio_output_name = "FlexFX Audio Out"; // USB audio output name
const char* usb_audio_input_name  = "FlexFX Audio In";  // USB audio input name
const char* usb_midi_output_name  = "FlexFX MIDI Out";  // USB MIDI output name
const char* usb_midi_input_name   = "FlexFX MIDI In";   // USB MIDI input name

const int audio_sample_rate     = 192000; // Audio sampling frequency
const int usb_output_chan_count = 2;      // 2 USB audio class 2.0 output channels
const int usb_input_chan_count  = 2;      // 2 USB audio class 2.0 input channels
const int i2s_channel_count     = 2;      // 2,4,or 8 I2S channels per SDIN/SDOUT wire

const int i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

// The control task is called at a rate of 1000 Hz and should be used to implement audio CODEC
// initialization/control, pot and switch sensing via I2C ADC's, handling of properties from USB
// MIDI, and generation of properties to be consumed by the USB MIDI host and by the DSP threads.
// The incoming USB property 'rcv_prop' is valid if its ID is non-zero. Outgoing USB and DSP props
// will be sent out if their ID is non-zero - but they are only available for routing if their ID's
// are zero (non-zero indicating that routing is still in progress). It's OK to use floating point
// calculations here since this thread is not a real-time audio or DSP thread.

void app_control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
}

// The mixer function is called once per audio sample and is used to route USB, I2S and DSP samples.
// This function should only be used to route samples and for very basic DSP processing - not for
// substantial sample processing since this may starve the I2S audio driver. Do not use floating
// point operations since this is a real-time audio thread - all DSP operations and calculations
// should be performed using fixed-point math.
// NOTE: IIR, FIR, and BiQuad coeff and state data *must* be declared non-static global!

void app_mixer( const int usb_output[32], int usb_input[32],
                const int i2s_output[32], int i2s_input[32],
                const int dsp_output[32], int dsp_input[32], const int property[6] )
{
}

// Audio Processing Threads. These functions run on tile 1 and are called once for each audio sample
// cycle. They cannot share data with the controller task or the mixer functions above that run on
// tile 0. The number of incoming and outgoing samples in the 'samples' array is set by the constant
// 'dsp_chan_count' defined above. Do not use floating point operations since these are real-time
// audio threads - all DSP operations and calculations should be performed using fixed-point math.
// NOTE: IIR, FIR, and BiQuad coeff and state data *must* be declared non-static global!

// Initialize DSP thread data (filter data and other algorithm data) here.
void app_initialize( void ) // Called once upon boot-up.
{
}

// Process samples (from the mixer function) and properties. Send results to stage 2.
void app_thread1( int samples[32], const int property[6] )
{
}

// Process samples (from stage 1) and properties. Send results to stage 3.
void app_thread2( int samples[32], const int property[6] )
{
}

// Process samples (from stage 2) and properties. Send results to stage 4.
void app_thread3( int samples[32], const int property[6] )
{
}

// Process samples (from stage 3) and properties. Send results to stage 5.
void app_thread4( int samples[32], const int property[6] )
{
}

// Process samples (from stage 4) and properties. Send results to the mixer function.
void app_thread5( int samples[32], const int property[6] )
{
}
