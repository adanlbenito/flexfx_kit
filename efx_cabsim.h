#include "flexfx.h"

#define PROP_CABSIM_OUTPUT   0x33020001 // Out Volume L, Out Volume R, 0, 0, 0
#define PROP_CABSIM_IR_BEG   0x33020002 // Start IR loading
#define PROP_CABSIM_IR_DATA  0x33020003 // Load five IR samples
#define PROP_CABSIM_IR_END   0x33020004 // End IR loadinbg

void efx_cabsim__control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] );

void efx_cabsim__mixer( const int* usb_output, int* usb_input,
                        const int* i2s_output, int* i2s_input,
                        const int* dsp_output, int* dsp_input, const int* property );

void efx_cabsim__dsp_thread1( int* samples, const int* property );
void efx_cabsim__dsp_thread2( int* samples, const int* property );
void efx_cabsim__dsp_thread3( int* samples, const int* property );
void efx_cabsim__dsp_thread4( int* samples, const int* property );
void efx_cabsim__dsp_thread5( int* samples, const int* property );
