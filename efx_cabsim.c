#include <math.h>
#include <string.h>
#include "efx_cabsim.h"

const char* product_name_string    = "FlexFX Cabsim";  // Your company/product name
const int   target_hardware        = 0;         // 0 for FlexFX Digital/ComboA, 1 for FlexFX ComboB
const int   audio_sample_rate      = 48000;     // Default sample rate at boot-up
const int   usb_output_chan_count  = 2;         // 2 USB audio class 2.0 output channels
const int   usb_input_chan_count   = 2;         // 2 USB audio class 2.0 input channels
const int   i2s_channel_count      = 2;         // ADC/DAC channels per SDIN/SDOUT wire (2,4,or 8)
const int   i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

//const char  interface_string[] = "No interface is specified";
//const char  controller_string[] = "No controller is available";

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    efx_cabsim__control( rcv_prop, usb_prop, dsp_prop );
}

void mixer( const int* usb_output, int* usb_input,
            const int* i2s_output, int* i2s_input,
            const int* dsp_output, int* dsp_input, const int* property )
{
    efx_cabsim__mixer( usb_output, usb_input, i2s_output, i2s_input,
                       dsp_output, dsp_input, property );
}

void dsp_thread1( int* samples, const int* property )
{
    efx_cabsim__dsp_thread1( samples, property );
}

void dsp_thread2( int* samples, const int* property )
{
    efx_cabsim__dsp_thread2( samples, property );
}

void dsp_thread3( int* samples, const int* property )
{
    efx_cabsim__dsp_thread3( samples, property );
}

void dsp_thread4( int* samples, const int* property )
{
    efx_cabsim__dsp_thread4( samples, property );
}

void dsp_thread5( int* samples, const int* property )
{
    efx_cabsim__dsp_thread5( samples, property );
}

