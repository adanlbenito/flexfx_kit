#include "flexfx.h"
#include <math.h>
#include <string.h>

const char* company_name_string    = "FlexFX";  // Your company name
const char* product_name_string    = "Example"; // Your product name
const int   audio_sample_rate      = 48000;     // Audio sampling frequency
const int   usb_output_chan_count  = 2;         // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;         // 2 USB audio class 2.0 input channels
const int   i2s_channel_count      = 2;         // ADC/DAC channels per SDIN/SDOUT wire

const int   i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

#define PROP_PRODUCT_ID      0x0101                  // Your product ID, must not be 0x0000!
#define PROP_EXAMPLE_PROPS   (PROP_PRODUCT_ID << 16) // Base property ID value
#define PROP_EXAMPLE_IR_BEG  (PROP_EXAMPLE_PROPS+1)  // Start IR loading
#define PROP_EXAMPLE_IR_DATA (PROP_EXAMPLE_PROPS+2)  // Load five IR samples
#define PROP_EXAMPLE_IR_END  (PROP_EXAMPLE_PROPS+3)  // End IR loadinbg

#define IR_PROP_ID(xx)  ((xx) & 0xFFFF8000) // IR property ID has two parts (ID and offset)
#define IR_PROP_IDX(xx) ((xx) & 0x00000FFF) // Up to 5*0xFFF samples (5 samples per property)

void copy_prop( int dst[6], const int src[6] )
{
    dst[0]=src[0]; dst[1]=src[1]; dst[2]=src[2]; dst[3]=src[3]; dst[4]=src[4]; dst[5]=src[5];
}

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || dsp_prop[0] != 0 ) return;

    // Pass cabsim IR data to the DSP threads if usb and dsp send properties are available for use.
    if( rcv_prop[0] == PROP_EXAMPLE_IR_BEG || rcv_prop[0] == PROP_EXAMPLE_IR_END ||
        rcv_prop[0] == PROP_EXAMPLE_IR_DATA )
    {
        copy_prop( dsp_prop, rcv_prop ); // Send to DSP threads.
        copy_prop( usb_prop, rcv_prop ); // Send to USB host as an acknowledge and for flow-control.
        rcv_prop[0] = 0; // Mark the incoming USB property as consumed (allow RX of the next prop).
    }
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

int ir_coeff[2400], ir_state[2400]; // DSP data *must* be non-static global!

void dsp_initialize( void ) // Called once upon boot-up.
{
    memset( &ir_coeff, 0, sizeof(ir_coeff) );
    memset( &ir_state, 0, sizeof(ir_state) );
    ir_coeff[0] = ir_coeff[1200] = FQ(+1.0);
}

void dsp_thread1( int* samples, const int* property )
{
    static int offset = 0;
    if( property[0] == PROP_EXAMPLE_IR_BEG ) offset = 0;
    // Check for properties containing new cabsim IR data, save new data to RAM
    if( property[0] == PROP_EXAMPLE_IR_DATA )
    {
        ir_coeff[offset+0] = property[1] / 32; ir_coeff[offset+1] = property[2] / 32;
        ir_coeff[offset+2] = property[3] / 32; ir_coeff[offset+3] = property[4] / 32;
        ir_coeff[offset+4] = property[5] / 32; offset += 5;
    }
    samples[2] = 0; samples[3] = 1 << 31; // Initial 64-bit Q1.63 accumulator value
    samples[4] = 0; samples[5] = 1 << 31; // Initial 64-bit Q1.63 accumulator value
    // Perform 240-sample convolution (1st 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*0, ir_state+240*0, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*5, ir_state+240*5, samples+4,samples+5 );
}

void dsp_thread2( int* samples, const int* property )
{
    // Perform 240-sample convolution (2nd 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*1, ir_state+240*1, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*6, ir_state+240*6, samples+4,samples+5 );
}

void dsp_thread3( int* samples, const int* property )
{
    // Perform 240-sample convolution (3rd 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*2, ir_state+240*2, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*7, ir_state+240*7, samples+4,samples+5 );
}

void dsp_thread4( int* samples, const int* property )
{
    // Perform 240-sample convolution (4th 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*3, ir_state+240*3, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*8, ir_state+240*8, samples+4,samples+5 );
}

void dsp_thread5( int* samples, const int* property )
{
    static bool muted = 0;
    // Check IR property -- Mute at start of new IR loading, un-mute when done.
    if( property[0] == PROP_EXAMPLE_IR_BEG ) muted = 1;
    if( property[0] == PROP_EXAMPLE_IR_END ) muted = 0;
    // Perform 240-sample convolution (5th and last 240 of 1220 total) of sample with IR data
    samples[0] = dsp_convolve( samples[0], ir_coeff+240*4, ir_state+240*4, samples+2,samples+3 );
    samples[1] = dsp_convolve( samples[1], ir_coeff+240*9, ir_state+240*9, samples+4,samples+5 );
    // Extract 32-bit Q28 from 64-bit Q63 and then apply mute/un-mute based on IR loading activity.
    DSP_EXT( samples[0], samples[2], samples[3] );
    DSP_EXT( samples[1], samples[4], samples[5] );
    samples[0] = muted ? 0 : samples[0];
    samples[1] = muted ? 0 : samples[1];
}
