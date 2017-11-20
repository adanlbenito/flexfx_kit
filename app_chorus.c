#include "flexfx.h" // Defines config variables, I2C and GPIO functions, DSP functions, etc.
#include <math.h>   // Floating point for filter coeff calculations in the background process.
#include <string.h>

// System configuration ...

const char* company_name_string    = "FlexFX";        // Your company name
const char* product_name_string    = "FlexFX Chorus"; // Your product name
const int   default_sample_rate    = 48000; // Default sample rate at boot-up
const int   supported_sample_rates = 0x02;  // 1=44k1, 2=48k0, 4=88k2, ... 32=192k0
const int   dsp_channel_count      = 32;    // 32 channels to/from each DSP thread
const int   usb_output_chan_count  = 2;     // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;     // 2 USB audio class 2.0 input channels
const int   i2s_tdm_slot_count     = 2;     // 2 for I2S (Stereo), 4 for I4S, 8 = I8S
const int   i2s_sync_word_format   = 0;     // 0 for PCM, 1 for I2S

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
    dsp_input[0] = i2s_output[6]/2 - i2s_output[7]/2; // DSP left input channel = guitar input.
    dsp_input[1] = i2s_output[6]/2 - i2s_output[7]/2; // DSP right input channel = guitar input.
    i2s_input[6] = dsp_output[0]; // Line out left channel = DSP output left channel
    i2s_input[7] = dsp_output[1]; // Line out right channel = DSP output right channel
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

// Process samples (from stage 1) and properties. Send results to stage 3.
void dsp_thread2( int* samples, const int* property )
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

// Process samples (from stage 2) and properties. Send results to stage 4.
void dsp_thread3( int* samples, const int* property )
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

// Process samples (from stage 3) and properties. Send results to stage 5.
void dsp_thread4( int* samples, const int* property )
{
    int blend1 = FQ(+0.50), blend2 = FQ(+0.30);;
    // Mix dry signal with wet #1 and wet #2 and send to both left and right channels (0 and 1).
    samples[2] = dsp_multiply(samples[0],FQ(1.0)-blend1)/2 + dsp_multiply(samples[2],blend1)/2;
    samples[3] = dsp_multiply(samples[0],FQ(1.0)-blend2)/2 + dsp_multiply(samples[3],blend2)/2;
    samples[0] = samples[1] = samples[2]/2 + samples[3]/2;
}

// Process samples (from stage 4) and properties. Send results to the mixer function.
void dsp_thread5( int* samples, const int* property )
{
}
