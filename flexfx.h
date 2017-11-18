#include <platform.h>

typedef unsigned char byte;
typedef unsigned int  bool;

// Flash memory functions for data persistance (do not use these in real-time DSP threads).

extern void flash_open ( void );                   // Open the flash device
extern void flash_seek ( int page_number );        // Set the page for the next write/read
extern void flash_read ( byte buffer[256] );       // Read next flash page, move to the next page
extern void flash_write( const byte buffer[256] ); // Write next flash page, move to the next page
extern void flash_close( void );                   // Close the flash device

// Port I/O functions ...

extern void port_write( int portnum, bool value ); // Write binary value to port
extern bool port_read ( int portnum );             // Read binary value from port

// I2C functions for peripheral control (do not use these in real-time DSP threads).

extern void i2c_start( int speed );    // Set bit rate, assert an I2C start condition.
extern byte i2c_write( byte value );   // Write 8-bit data value.
extern byte i2c_read ( void );         // Read 8-bit data value.
extern void i2c_ack  ( byte ack );     // Assert the ACK/NACK bit after a read.
extern void i2c_stop ( void );         // Assert an I2C stop condition.

// FQ converts Q28 fixed-point to floating point
// QF converts floating-point to Q28 fixed-point

#define FQ(hh) (((hh)<0.0)?((int)((double)(1u<<28)*(hh)-0.5)):((int)(((double)(1u<<28)-1)*(hh)+0.5)))
#define QF(xx) (((int)(xx)<0)?((double)(int)(xx))/(1u<<28):((double)(xx))/((1u<<28)-1))

// MAC performs 32x32 multiply and 64-bit accumulation, SAT saturates a 64-bit result, EXT converts
// a 64-bit result to a 32-bit value (extract 32 from 64), LD2/ST2 loads/stores two 32-values
// from/to 64-bit aligned 32-bit data arrays at address PP. All 32-bit fixed-point values are Q28
// fixed-point formatted.
//
// AH (high) and AL (low) form the 64-bit signed accumulator
// XX, YY, and AA are 32-bit Q28 fixed point values
// PP is a 64-bit aligned pointer to two 32-bit Q28 values

#define DSP_MAC( ah, al, xx, yy ) asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(yy),"0"(ah),"1"(al) );
#define DSP_SAT( ah, al )         asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(31),"0"(ah),"1"(al));
#define DSP_EXT( xx, ah, al )     asm("lextract %0,%1,%2,%3,32":"=r"(xx):"r"(ah),"r"(al),"r"(28));
#define DSP_LD2( pp, xx, yy )     asm("ldd %0,%1,%2[0]":"=r"(xx),"=r"(yy):"r"(pp));
#define DSP_ST2( pp, xx, yy )     asm("std %0,%1,%2[0]"::"r"(xx), "r"(yy),"r"(pp));

inline int dsp_multiply( int xx, int yy ) // RR = XX * YY
{
    int ah = 0; unsigned al = 1<<(28-1);
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(yy),"0"(ah),"1"(al) );
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(28));
    return ah;
}
inline int dsp_mult_acc( int xx, int yy, int aa ) // RR = XX * YY + AA
{
    int ah = aa; unsigned al = 0;
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(yy),"0"(ah),"1"(al) );
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(28));
    return ah;
}

// Math and filter functions.
//
// XX, CC, SS, Yn, and AA are 32-bit fixed point samples/data in Q28 format
// DD is the distance (0<=DD<1) between the first two points for interpolation
// Yn are the data points to be interpolated
// NN is FIR filter tap-count for 'fir', 'upsample', 'dnsample' and 'convolve' functions
// NN is IIR filter order or or IIR filter count for cascaded IIR's
// CC is array of 32-bit filter coefficients - length is 'nn' for FIR, nn * 5 for IIR
// SS is array of 32-bit filter state - length is 'nn' for FIR, nn * 4 for IIR, 1 for LPF/HPF/INT
// KK is time constant for LPF, HPF and INT where KK = exp(-2.0 * PI * Fc)
// AH (high) and AL (low) form the 64-bit signed accumulator

int dsp_random   ( int gg, int seed );                       // Random number, gg = previous value
int dsp_interp   ( int dd, int y1, int y2 );                 // Linear interpolation
int dsp_lagrange ( int dd, int y1, int y2, int y3 );         // Lagrange interpolation
int dsp_integrate( int xx, int kk, int* ss );                // Leaky integrator
int dsp_fir_filt ( int xx, const int* cc, int* ss, int nn ); // FIR filter of nn taps
int dsp_iir_filt ( int xx, const int* cc, int* ss, int nn ); // nn Cascaded bi-quad IIR filters
int dsp_convolve ( int xx, const int* cc, int* ss, int* ah, int* al ); // 240 tap FIR convolution

// Biquad filter coefficient calculation functions (do not use these in real-time DSP threads).
//
// CC is an array of floating point filter coefficients
// FF, QQ, GG are the filter frequency, filter Q-factor, and filter gain
// For low/high pass filters set QQ to zero to create a single-pole 6db/octave filter

void make_notch    ( int cc[5], double ff, double qq );
void make_lowpass  ( int cc[5], double ff, double qq );
void make_highpass ( int cc[5], double ff, double qq );
void make_allpass  ( int cc[5], double ff, double qq );
void make_bandpass ( int cc[5], double ff1, double ff2 );
void make_peaking  ( int cc[5], double ff, double qq, double gg );
void make_lowshelf ( int cc[5], double ff, double qq, double gg );
void make_highshelf( int cc[5], double ff, double qq, double gg );
