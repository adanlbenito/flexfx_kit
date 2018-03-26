#include "flexfx.h"

#define PROP_CABSIM_OUTPUT   0x33020001 // Out Volume L, Out Volume R, 0, 0, 0
#define PROP_CABSIM_IR_BEG   0x33020002 // Start IR loading
#define PROP_CABSIM_IR_DATA  0x33020003 // Load five IR samples
#define PROP_CABSIM_IR_END   0x33020004 // End IR loadinbg

void efx_cabsim__app_control( const int rcv_prop[6], int usb_prop[6], int dsp_prop[6] );

void efx_cabsim__app_mixer( const int usb_output[32], int usb_input[32],
                            const int i2s_output[32], int i2s_input[32],
                            const int dsp_output[32], int dsp_input[32], const int property[6] );

void efx_cabsim__app_thread1( int samples[32], const int property[6] );
void efx_cabsim__app_thread2( int samples[32], const int property[6] );
void efx_cabsim__app_thread3( int samples[32], const int property[6] );
void efx_cabsim__app_thread4( int samples[32], const int property[6] );
void efx_cabsim__app_thread5( int samples[32], const int property[6] );
