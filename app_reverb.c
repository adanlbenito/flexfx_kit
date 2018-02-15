#include "flexfx.h"
#include <math.h>
#include <string.h>

const char* product_name_string   = "FlexFX Example"; // Your company/product name
const int   audio_sample_rate     = 48000;     // Audio sampling frequency
const int   usb_output_chan_count = 2;         // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count  = 2;         // 2 USB audio class 2.0 input channels
const int   i2s_channel_count     = 2;         // ADC/DAC channels per SDIN/SDOUT wire
const char  interface_string[]    = "No interface is specified";
const char  controller_string[]   = "No controller is available";

const int   i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || dsp_prop[0] != 0 ) return;
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

void dsp_thread1( int* samples, const int* property )
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

void dsp_thread2( int* samples, const int* property )
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

void dsp_thread3( int* samples, const int* property )
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

void dsp_thread4( int* samples, const int* property )
{
}

void dsp_thread5( int* samples, const int* property )
{
}
