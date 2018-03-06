#include "flexfx.h"
#include <math.h>
#include <string.h>

const char* company_name_string   = "FlexFX";  // Your company name
const char* product_name_string   = "Example"; // Your product name
const char* usb_audio_output_name = "FlexFX Audio Output"; // USB audio output endpoint name
const char* usb_audio_input_name  = "FlexFX Audio In put"; // USB audio input endpoint name
const char* usb_midi_output_name  = "FlexFX MIDI Output";  // USB MIDI output endpoint name
const char* usb_midi_input_name   = "FlexFX MIDI In put";  // USB MIDI input endpoint name

const int audio_sample_rate     = 192000; // Audio sampling frequency
const int usb_output_chan_count = 2;      // 2 USB audio class 2.0 output channels
const int usb_input_chan_count  = 2;      // 2 USB audio class 2.0 input channels
const int i2s_channel_count     = 2;      // 2,4,or 8 I2S channels per SDIN/SDOUT wire

const int i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || dsp_prop[0] != 0 ) return;
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

// util_iir.py highpass 0.0004 0.707 0, util_iir.py peaking 0.002 0.707 +6.0
int emphasis1_state[8] = {0,0,0,0,0,0,0,0}, emphasis1_coeff[10] =
{
    FQ(+0.998224158),FQ(-1.996448315),FQ(+0.998224158),FQ(+1.996445162),FQ(-0.996451468),
    FQ(+1.006222470),FQ(-1.987338895),FQ(+0.981273349),FQ(+1.987338895),FQ(-0.987495819),
};
// util_iir.py highpass 0.0002 0.707 0, util_iir.py peaking 0.002 0.707 +6.0
int emphasis2_state[8] = {0,0,0,0,0,0,0,0}, emphasis2_coeff[10] =
{
    FQ(+1.006222470),FQ(-1.987338895),FQ(+0.981273349),FQ(+1.987338895),FQ(-0.987495819),
    FQ(+1.006222470),FQ(-1.987338895),FQ(+0.981273349),FQ(+1.987338895),FQ(-0.987495819),
};
// util_iir.py highpass 0.0001 0.707 0, util_iir.py peaking 0.002 0.707 +6.0
int emphasis3_state[8] = {0,0,0,0,0,0,0,0}, emphasis3_coeff[10] =
{
    FQ(+1.003121053),FQ(-1.993688826),FQ(+0.990607128),FQ(+1.993688826),FQ(-0.993728181),
    FQ(+1.006222470),FQ(-1.987338895),FQ(+0.981273349),FQ(+1.987338895),FQ(-0.987495819),
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

int preamp1[4+3], preamp2[4+3], preamp3[4+3];

int preamp_model( int xx, int gain, int bias, int slewlim, int* state )
{
    // Add bias to input signal and apply additional gain (total preamp gain = 8 * gain)
    xx = dsp_multiply( xx + bias, gain );
    // Table lookup
    if( xx >= 0 ) {
        int ii = (xx & 0xFFFFC000) >> 14, ff = (xx & 0x00003FFF) << 14;
        if( ii > 16381 ) ii = 16381;
        xx = dsp_lagrange( ff, dsp_tanh_14[ii+0], dsp_tanh_14[ii+1], dsp_tanh_14[ii+2] );
    } else {
        int ii = (-xx & 0xFFFFC000) >> 14, ff = (-xx & 0x00003FFF) << 14;
        if( ii > 16381 ) ii = 16381;
        xx = -dsp_lagrange( ff, dsp_nexp_14[ii+0], dsp_nexp_14[ii+1], dsp_nexp_14[ii+2] );
    }
    // Slew rate limit and invert
    if( xx > state[6] + slewlim ) { xx = state[6] + slewlim; state[6] = xx; }
    if( xx < state[6] - slewlim ) { xx = state[6] - slewlim; state[6] = xx; }
    return -xx;
}

void dsp_initialize( void ) // Called once upon boot-up.
{
    memset( antialias_state1, 0, sizeof(antialias_state1) );
    memset( antialias_state2, 0, sizeof(antialias_state2) );
}

void dsp_thread1( int* samples, const int* property ) // Upsample
{
    // Up-sample by 2x by inserting zeros then apply the anti-aliasing filter
    samples[0] = 4 * dsp_fir( samples[0], antialias_coeff, antialias_state1, 64 );
    samples[1] = 4 * dsp_fir( 0,              antialias_coeff, antialias_state1, 64 );
}

void dsp_thread2( int* samples, const int* property ) // Preamp stage 1
{
    // Perform stage 1 overdrive on the two up-sampled samples for the left channel.
    samples[0] = dsp_iir     ( samples[0], emphasis1_coeff, emphasis1_state, 2 );
    samples[0] = preamp_model( samples[0], FQ(1.3), FQ(+0.0), FQ(0.4), preamp1 );
    samples[0] = dsp_iir     ( samples[0], lowpass1_coeff, lowpass1_state, 1 );
    samples[1] = dsp_iir     ( samples[1], emphasis1_coeff, emphasis1_state, 2 );
    samples[1] = preamp_model( samples[1], FQ(1.3), FQ(+0.0), FQ(0.4), preamp1 );
    samples[1] = dsp_iir     ( samples[1], lowpass1_coeff, lowpass1_state, 1 );
}

void dsp_thread3( int* samples, const int* property ) // Preamp stage 2
{
    // Perform stage 2 overdrive on the two up-sampled samples for the left channel.
    samples[0] = dsp_iir     ( samples[0], emphasis2_coeff, emphasis2_state, 2 );
    samples[0] = preamp_model( samples[0], FQ(1.0), FQ(+0.0), FQ(0.3), preamp2 );
    samples[0] = dsp_iir     ( samples[0], lowpass2_coeff, lowpass2_state, 1 );
    samples[1] = dsp_iir     ( samples[1], emphasis2_coeff, emphasis2_state, 2 );
    samples[1] = preamp_model( samples[1], FQ(1.0), FQ(+0.0), FQ(0.3), preamp2 );
    samples[1] = dsp_iir     ( samples[1], lowpass2_coeff, lowpass2_state, 1 );
}

void dsp_thread4( int* samples, const int* property ) // Preamp stage 3
{
    // Perform stage 3 overdrive on the two up-sampled samples for the left channel.
    samples[0] = dsp_iir     ( samples[0], emphasis3_coeff, emphasis3_state, 2 );
    samples[0] = preamp_model( samples[0], FQ(0.7), FQ(+0.0), FQ(0.2), preamp3 );
    samples[0] = dsp_iir     ( samples[0], lowpass3_coeff, lowpass3_state, 1 );
    samples[1] = dsp_iir     ( samples[1], emphasis3_coeff, emphasis3_state, 2 );
    samples[1] = preamp_model( samples[1], FQ(0.7), FQ(+0.0), FQ(0.2), preamp3 );
    samples[1] = dsp_iir     ( samples[1], lowpass3_coeff, lowpass3_state, 1 );
}

void dsp_thread5( int* samples, const int* property ) // Downsample
{
    // Down-sample by 2x by band-limiting via anti-aliasing filter and then discarding 1 sample.
    samples[0] = dsp_fir( samples[0], antialias_coeff, antialias_state2, 64 );
                     dsp_fir( samples[1], antialias_coeff, antialias_state2, 64 );
}
