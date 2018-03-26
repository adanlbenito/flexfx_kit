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

void copy_prop( int dst[6], const int src[6] )
{
    dst[0]=src[0]; dst[1]=src[1]; dst[2]=src[2]; dst[3]=src[3]; dst[4]=src[4]; dst[5]=src[5];
}

void app_control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    // Pass cabsim IR data to the DSP threads if usb and dsp send properties are available for use.
    //if( dsp_prop[0] == 0x9000 ) exit(0);
    copy_prop( dsp_prop, rcv_prop ); // Send to DSP threads.
}

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

int ir_coeff[2400], ir_state[2400]; // DSP data *must* be non-static global!

void app_thread1( int samples[32], const int property[6] )
{
    static int first = 1;
    if( first ) { first = 0; ir_coeff[0] = ir_coeff[1200] = FQ(+1.0); }
    // Check for properties containing new cabsim IR data, save new data to RAM
    if( (property[0] & 0xF000) == 0x9000 )
    {
    	int offset = 5 * (property[0] & 0x0FFF);
    	if( offset <= 2400-5 ) {
			ir_coeff[offset+0] = property[1] / 32; ir_coeff[offset+1] = property[2] / 32;
			ir_coeff[offset+2] = property[3] / 32; ir_coeff[offset+3] = property[4] / 32;
			ir_coeff[offset+4] = property[5] / 32;
		}
    }
    samples[2] = 0; samples[3] = 1 << 31; // Initial 64-bit Q1.63 accumulator value
    samples[4] = 0; samples[5] = 1 << 31; // Initial 64-bit Q1.63 accumulator value
    // Perform 240-sample convolution (1st 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*0, ir_state+240*0, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*5, ir_state+240*5, samples+4,samples+5 );
}

void app_thread2( int samples[32], const int property[6] )
{
    // Perform 240-sample convolution (2nd 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*1, ir_state+240*1, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*6, ir_state+240*6, samples+4,samples+5 );
}

void app_thread3( int samples[32], const int property[6] )
{
    // Perform 240-sample convolution (3rd 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*2, ir_state+240*2, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*7, ir_state+240*7, samples+4,samples+5 );
}

void app_thread4( int samples[32], const int property[6] )
{
    // Perform 240-sample convolution (4th 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*3, ir_state+240*3, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*8, ir_state+240*8, samples+4,samples+5 );
}

void app_thread5( int samples[32], const int property[6] )
{
    static bool muted = 0;
    // Check IR property -- Mute at start of new IR loading, un-mute when done.
    if( property[0] == 0x9000 ) muted = 1;
    if( property[0] == 0x9000 + 480 ) muted = 0;
    // Perform 240-sample convolution (5th and last 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*4, ir_state+240*4, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*9, ir_state+240*9, samples+4,samples+5 );
    // Extract 32-bit Q28 from 64-bit Q63 and then apply mute/un-mute based on IR loading activity.
    DSP_EXT( samples[0], samples[2], samples[3] );
    DSP_EXT( samples[1], samples[4], samples[5] );
    samples[0] = muted ? 0 : samples[0];
    samples[1] = muted ? 0 : samples[1];
}
