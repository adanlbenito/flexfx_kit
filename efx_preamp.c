#include "efx_cabsim.h" // FlexFX Preamp
#include <math.h>       // Floating point for filter coeff calculations in the background process.
#include <string.h>     // Memory and string functions

const char* product_name_string   = "FlexFX Preamp";    // Your product name
const char* usb_audio_output_name = "FlexFX Audio Out"; // USB audio output endpoint name
const char* usb_audio_input_name  = "FlexFX Audio In";  // USB audio input endpoint name
const char* usb_midi_output_name  = "FlexFX MIDI Out";  // USB MIDI output endpoint name
const char* usb_midi_input_name   = "FlexFX MIDI In";   // USB MIDI input endpoint name

const int audio_sample_rate     = 192000; // Default sample rate at boot-up
const int usb_output_chan_count = 2;      // 2 USB audio class 2.0 output channels
const int usb_input_chan_count  = 2;      // 2 USB audio class 2.0 input channels
const int i2s_channel_count     = 2;      // ADC/DAC channels per SDIN/SDOUT wire (2,4,or 8)

//const char interface_string[]  = "No interface is specified";
//const char controller_string[] = "No controller is available";

const int i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    // Pass incoming properties on to PREAMP control ...
    efx_cabsim__control( rcv_prop, usb_prop, dsp_prop );
    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || dsp_prop[0] != 0 ) return;
}

void mixer( const int* usb_output, int* usb_input,
            const int* i2s_output, int* i2s_input,
            const int* dsp_output, int* dsp_input, const int* property )
{
    efx_preamp__mixer( usb_output, usb_input, i2s_output, i2s_input,
                       dsp_output, dsp_input, property );
}

void dsp_thread1( int* samples, const int* property )
{
    efx_preamp__dsp_thread1( samples, property );
}

void dsp_thread2( int* samples, const int* property )
{
    efx_preamp__dsp_thread2( samples, property );
}

void dsp_thread3( int* samples, const int* property )
{
    efx_preamp__dsp_thread3( samples, property );
}

void dsp_thread4( int* samples, const int* property )
{
    efx_preamp__dsp_thread4( samples, property );
}

void dsp_thread5( int* samples, const int* property )
{
    efx_preamp__dsp_thread5( samples, property );
}
