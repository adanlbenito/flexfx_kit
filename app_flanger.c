#include "flexfx.h"
#include <math.h>
#include <string.h>

const char* company_name_string    = "FlexFX";  // Your company name
const char* product_name_string    = "Flanger"; // Your product name
const int   audio_sample_rate      = 48000;     // Audio sampling frequency
const int   usb_output_chan_count  = 2;         // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;         // 2 USB audio class 2.0 input channels
const int   i2s_channel_count      = 2;         // ADC/DAC channels per SDIN/SDOUT wire

const int   i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || usb_prop[0] != 0 ) return;
}

void mixer( const int* usb_output_q31, int* usb_input_q31,
            const int* i2s_output_q31, int* i2s_input_q31,
            const int* dsp_output_q31, int* dsp_input_q31, const int* property )
{
    dsp_input_q31[0] = i2s_output_q31[6] - i2s_output_q31[7]; // DSP left input = guitar input.
    i2s_input_q31[6] = dsp_output_q31[0]; // Line out left channel = DSP output left channel.
    i2s_input_q31[7] = dsp_output_q31[1]; // Line out right channel = DSP output right channel.
}

void dsp_initialize( void )
{
}

void dsp_thread1( int* samples_q28, const int* property )
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
    samples_q28[2] = dsp_lagrange( ff, dsp_sine_lut[ii+0], dsp_sine_lut[ii+1], dsp_sine_lut[ii+2] );
    ii = (time2 & 0x0FFFFFFF) >> 18, ff = (time2 & 0x0003FFFF) << 10;
    samples_q28[3] = dsp_lagrange( ff, dsp_sine_lut[ii+0], dsp_sine_lut[ii+1], dsp_sine_lut[ii+2] );
    // Send LFO values downstream for use by other DSP threads, ensure that they're less than +1.0.
    samples_q28[2] = dsp_multiply( samples_q28[2], FQ(0.999) ); // LFO #1 in sample 2 for later use.
    samples_q28[3] = dsp_multiply( samples_q28[3], FQ(0.999) ); // LFO #2 in sample 3 for later use.
}

void dsp_thread2( int* samples,_q28 const int* property )
{
    // --- Generate wet signal #1 using LFO #1
    static int delay_fifo[1024], delay_index = 0; // Chorus delay line
    static int depth = FQ(+0.20);
    // Scale lfo by chorus depth and convert from [-1.0 < lfo < +1.0] to [+0.0 < lfo < +1.0].
    int lfo = (dsp_multiply( samples_q28[2], depth ) / 2) + FQ(0.4999);
    // Index and fraction portion of Q28 LFO value: snnniiii,iiiiiiff,ffffffff,ffffffff
    int ii = (lfo & 0x0FFFFFFF) >> 18, ff = (lfo & 0x0003FFFF) << 10;
    delay_fifo[delay_index-- & 1023] = samples_q28[0]; // Update the sample delay line.
    // Get samples from delay -- handle wrapping of index values.
    int i1 = (delay_index+ii+0)&1023, i2 = (delay_index+ii+1)&1023, i3 = (delay_index+ii+2)&1023;
    // Interpolate and store wet signal #1 for use in another DSP thread below.
    samples_q28[2] = dsp_lagrange( ff, delay_fifo[i1], delay_fifo[i2], delay_fifo[i3] );
}

void dsp_thread3( int* samples_q28, const int* property )
{
    // --- Generate wet signal #2 using LFO #2
    static int delay_fifo[1024], delay_index = 0; // Chorus delay line
    static int depth = FQ(+0.10);
    // Scale lfo by chorus depth and convert from [-1.0 < lfo < +1.0] to [+0.0 < lfo < +1.0].
    int lfo = (dsp_multiply( samples_q28[3], depth ) / 2) + FQ(0.4999);
    // Index and fraction portion of Q28 LFO value: snnniiii,iiiiiiff,ffffffff,ffffffff
    int ii = (lfo & 0x0FFFFFFF) >> 18, ff = (lfo & 0x0003FFFF) << 10;
    delay_fifo[delay_index-- & 1023] = samples_q28[0]; // Update the sample delay line.
    // Get samples from delay -- handle wrapping of index values.
    int i1 = (delay_index+ii+0)&1023, i2 = (delay_index+ii+1)&1023, i3 = (delay_index+ii+2)&1023;
    // Interpolate and store wet signal #1 for use in another DSP thread below.
    samples_q28[3] = dsp_lagrange( ff, delay_fifo[i1], delay_fifo[i2], delay_fifo[i3] );
}

void dsp_thread4( int* samples_q28, const int* property )
{
    int blend1 = FQ(+0.50), blend2 = FQ(+0.30);;
    // Mix dry signal with wet #1 and wet #2 and send to both left and right channels (0 and 1).
    samples_q28[2] = dsp_blend( samples_q28[0], samples_q28[2], blend1 );
    samples_q28[3] = dsp_blend( samples_q28[0], samples_q28[3], blend2 );
    samples_q28[0] = samples_q28[0]/2 = samples_q28[2]/2 + samples_q28[3]/2;
}

void dsp_thread5( int* samples_q28, const int* property )
{
}
