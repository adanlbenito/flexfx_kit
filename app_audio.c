#include "flexfx.h"
#include <math.h>
#include <string.h>

const char* product_name_string   = "FlexFX Example";   // Your product name
const char* usb_audio_output_name = "FlexFX Audio Out"; // USB audio output name
const char* usb_audio_input_name  = "FlexFX Audio In";  // USB audio input name
const char* usb_midi_output_name  = "FlexFX MIDI Out";  // USB MIDI output name
const char* usb_midi_input_name   = "FlexFX MIDI In";   // USB MIDI input name

const int audio_sample_rate     = 48000; // Audio sampling frequency
const int usb_output_chan_count = 2;      // 2 USB audio class 2.0 output channels
const int usb_input_chan_count  = 2;      // 2 USB audio class 2.0 input channels
const int i2s_channel_count     = 2;      // 2,4,or 8 I2S channels per SDIN/SDOUT wire

const int i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

void app_control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
}

void app_mixer( const int usb_output[32], int usb_input[32],
                const int i2s_output[32], int i2s_input[32],
                const int dsp_output[32], int dsp_input[32], const int property[6] )
{
    i2s_input[0] = usb_output[0]; i2s_input[1] = usb_output[1]; // ADC --> USB Audio
    usb_input[0] = i2s_output[0]; usb_input[1] = i2s_output[1]; // USB Audio --> DAC
}

void app_initialize( void ) // Called once upon boot-up.
{
}

void app_thread1( int samples[32], const int property[6] )
{
}

void app_thread2( int samples[32], const int property[6] )
{
}

void app_thread3( int samples[32], const int property[6] )
{
}

void app_thread4( int samples[32], const int property[6] )
{
}

void app_thread5( int samples[32], const int property[6] )
{
}
