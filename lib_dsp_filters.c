// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "lib_dsp_qformat.h"
#include "lib_dsp_math.h"
#include "lib_dsp_vector.h"
#include "lib_dsp_statistics.h"
#include "lib_dsp_filters.h"

/** This function implements a Finite Impulse Response (FIR) filter.
 *  
 *  The function operates on a single sample of input and output data (i.e.
 *  each call to the function processes one sample).
 *
 *  The FIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``h[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[n]=x[n]*h[0]+x[n-1]*h[1]+x[n-2]*h[2]+x[n-N+1]*h[N-1]``
 *
 *  The parameter ``filter_coeffs`` points to a coefficient array of size
 *  N = ``num_taps``. The filter coefficients are stored in forward order
 *  (e.g. ``h[0],h[1],h[N-1]``).
 *
 *  The following example shows a five-tap (4th order) FIR filter with samples
 *  and coefficients represented in Q28 fixed-point format.
 *  \code
 *  int32_t filter_coeff[5] = { Q28(0.5),Q(-0.5),Q28(0.0),Q28(-0.5),Q28(0.5) };
 *  int32_t filter_state[4] = { 0, 0, 0, 0 };
 *  int32_t result = lib_dsp_fir( sample, filter_coeff, filter_state, 5, 28 );
 *  \endcode
 *
 *  The FIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coeffient and state data pair. Multiplication results
 *  are accumulated in 64-bit accumulator with the final result shifted
 *  to the required fixed-point format. Therefore overflow behavior of
 *  the 32-bit multiply operation and truncation behavior from final shifing
 *  of the accumulated multiplication results must be considered.
 *
 *  \param  input_sample    The new sample to be processed.
 *  \param  filter_coeffs   Pointer to FIR coefficients array arranged
 *                          as ``[b0,b1,b2,bN-1]``.
 *  \param  state_data      Pointer to filter state data array of length N.
 *                          Must be initialized at startup to all zero's.
 *  \param  tap_count       Filter tap count (N = ``tap_count`` = filter order + 1).
 *  \param q_format         Fixed point format (i.e. number of fractional bits).
 *  \returns                The resulting filter output sample.
 */

int32_t lib_dsp_filters_fir
(
    int32_t        input_sample,
    const int32_t* filter_coeffs,
    int32_t*       state_data,
    int32_t        tap_count,
    int32_t        q_format
) {
    int32_t ah = 0, b0, b1, s0 = input_sample, s1, s2, s3;
    uint32_t al = 1 << (q_format-1);
    
    while( tap_count >= 20 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[8]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[8]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[9]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[9]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        
        tap_count -= 20; filter_coeffs += 20; state_data += 20;
    }
    switch( tap_count )
    {
        case 19:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[8]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[8]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[18] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[18]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 18:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[16]; 
        asm("std %0,%1,%2[8]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 17:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        state_data[16] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[16]),"r"(s0),"0"(ah),"1"(al));
        break;

        case 16:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[7]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[7]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        break;
        
        case 15:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[14] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[14]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 14:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[12]; 
        asm("std %0,%1,%2[6]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 13:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        state_data[12] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[12]),"r"(s0),"0"(ah),"1"(al));
        break;

        case 12:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[5]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[5]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        break;
        
        case 11:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[10] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[10]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 10:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[8]; 
        asm("std %0,%1,%2[4]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 9:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        state_data[8] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[8]),"r"(s0),"0"(ah),"1"(al));
        break;

        case 8:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        break;
        
        case 7:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[6] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[6]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 6:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[4]; 
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 5:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));

        state_data[4] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[4]),"r"(s0),"0"(ah),"1"(al));
        break;

        case 4:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(state_data));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s2),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s3),"0"(ah),"1"(al));
        break;
        
        case 3:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        state_data[2] = s2;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[2]),"r"(s2),"0"(ah),"1"(al));
        break;

        case 2:
        
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
        s1 = state_data[0]; 
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(state_data));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 1:
        
        state_data[0] = s0;
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(filter_coeffs[0]),"r"(s0),"0"(ah),"1"(al));
        break;
    }
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
    return ah;
}

// FIR filter (even coeff array boundary, no state data shifting - for internal use only)

int32_t _lib_dsp_filters_interpolate__fir_even
(
    const int32_t* coeff,
    const int32_t* state,
    int32_t        taps,
    int32_t        format
) {
    int32_t ah = 0, b0, b1, s0, s1;
    uint32_t al = 1 << (format-1);
    
    while( taps >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[2]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[3]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        taps -= 8; coeff += 8; state += 8;
    }
    switch( taps )
    {        
        case 7:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[2]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[6]),"r"(state[6]),"0"(ah),"1"(al));
        break;

        case 6:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 5:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[4]),"r"(state[4]),"0"(ah),"1"(al));
        break;

        case 4:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;
        
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[2]),"r"(state[2]),"0"(ah),"1"(al));
        break;

        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 1:        
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[0]),"r"(state[0]),"0"(ah),"1"(al));
        break;
    }
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(format));
    return ah;
}

// FIR filter (odd coeff array boundary, no state data shifting - for internal use only)

int32_t _lib_dsp_filters_interpolate__fir_odd
(
    const int32_t* coeff,
    const int32_t* state,
    int32_t        taps,
    int32_t        format
) {
    int32_t ah = 0, b0, b1, s0, s1;
    uint32_t al = 1 << (format-1);
    
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[0]),"r"(state[0]),"0"(ah),"1"(al));
    --taps;

    while( taps >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[2]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[3]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        taps -= 8; coeff += 8; state += 8;
    }
    switch( taps )
    {        
        case 7:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[2]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[6]),"r"(state[6]),"0"(ah),"1"(al));
        break;

        case 6:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 5:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[4]),"r"(state[4]),"0"(ah),"1"(al));
        break;

        case 4:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[1]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;
        
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("ldd %0,%1,%2[0]":"=r"(s1),"=r"(s0):"r"(state));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[2]),"r"(state[2]),"0"(ah),"1"(al));
        break;

        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(coeff));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b0),"r"(s0),"0"(ah),"1"(al));
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(b1),"r"(s1),"0"(ah),"1"(al));
        break;

        case 1:        
        asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(coeff[0]),"r"(state[0]),"0"(ah),"1"(al));
        break;
    }
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(format));
    return ah;
}

/** This function implements an interpolating FIR filter.
 *
 *  The function operates on a single input sample and outputs a set of samples
 *  representing the interpolated data, whose sample count is equal to
 *  ``interp_factor``. (i.e. and each call to the function processes one sample
 *  and results in ``interp_factor`` output samples).
 * 
 *  The FIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``h[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[n]=x[n]*h[0]+x[n-1]*h[1]+x[n-2]*h[2]+x[n-N+1]*h[N-1]``
 * 
 *  ``filter_coeffs`` points to a coefficient array of size N = ``num_taps``.
 *  The filter coefficients
 *  are stored in forward order (e.g. ``h[0],h[1],h[N-1]``).
 * 
 *  The FIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in
 *  64-bit accumulater with the final result shifted to the required
 *  fixed-point format. Therefore overflow behavior of the 32-bit multiply
 *  operation and truncation behavior from final shifing of the accumulated
 *  multiplication results must be considered.
 * 
 *  \param input_sample    The new sample to be processed.
 *  \param filter_coeffs   Pointer to FIR coefficients array arranged as:
 *                         ``hM,h(1L+M),h(2L+M),h((N-1)L+M),``
 *                         ``h1,h(1L+1),h(2L+1),h((N-1)L+1),``
 *                         ``h0,h(1L+0),h(2L+0),h((N-1)L+0),``
 *                         where M = N-1
 *  \param state_data      Pointer to filter state data array of length N.
 *                         Must be initialized at startup to all zero's.
 *  \param  tap_count      Filter tap count (N = ``tap_count`` = filter order + 1).
 *  \param interp_factor   The interpolation factor/index (i.e. the up-sampling ratio).
 *                         The interpolation factor/index can range from 2 to 16.
 *  \param output_samples  The resulting interpolated samples.
 *  \param q_format        Fixed point format (i.e. number of fractional bits).
 */

void lib_dsp_filters_interpolate
(
    int32_t       input,
    const int32_t coeff[],
    int32_t       state[],
    int32_t       taps,
    int32_t       L,
    int32_t       output[],
    int32_t       q_format
) {
    int32_t s0 = input, s1, s2, s3;
    int32_t odd = 0, length = taps / L, len;
    int32_t* ss = state;

    /*
    L = 5, N = 3
    
           b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 bA bB bC bD bE
    y0 <-- x0  0  0  0  0 x1  0  0  0  0 x2  0  0  0  0
    y1 <--  0 x0  0  0  0  0 x1  0  0  0  0 x2  0  0  0
    y2 <--  0  0 x0  0  0  0  0 x1  0  0  0  0 x2  0  0
    y3 <--  0  0  0 x0  0  0  0  0 x1  0  0  0  0 x2  0
    y4 <--  0  0  0  0 x0  0  0  0  0 x1  0  0  0  0 x2

           b0 b5 bA b1 b6 bB b2 b7 bC b3 b8 bD b4 b9 bE
    y0 <-- x0 x1 x2  0  0  0  0  0  0  0  0  0  0  0  0
    y1 <--  0  0  0 x0 x1 x2  0  0  0  0  0  0  0  0  0
    y2 <--  0  0  0  0  0  0 x0 x1 x2  0  0  0  0  0  0
    y3 <--  0  0  0  0  0  0  0  0  0 x0 x1 x2  0  0  0
    y4 <--  0  0  0  0  0  0  0  0  0  0  0  0 x0 x1 x2
    */
    
    len = length;
    while( len >= 8 )
    {
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[3]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[3]"::"r"(s3), "r"(s2),"r"(ss));
        len -= 8; ss += 8;
    }
    switch( len )
    {
        case 7:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(ss));
        ss[6] = s2;
        break;

        case 6:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[2]"::"r"(s1), "r"(s0),"r"(ss));
        break;
    
        case 5:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        ss[4] = s0;
        break;
    
        case 4:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        asm("ldd %0,%1,%2[1]":"=r"(s0),"=r"(s3):"r"(ss));
        asm("std %0,%1,%2[1]"::"r"(s3), "r"(s2),"r"(ss));
        break;
    
        case 3:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        ss[2] = s2;
        break;
    
        case 2:
        asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss));
        asm("std %0,%1,%2[0]"::"r"(s1), "r"(s0),"r"(ss));
        break;
    
        case 1: ss[0] = s0; break;
    }
    
    for( int32_t i = 0; i < L; ++i )
    {
        if( odd )
            output[i] = _lib_dsp_filters_interpolate__fir_odd( coeff, state, length, q_format );
        else
            output[i] = _lib_dsp_filters_interpolate__fir_even( coeff, state, length, q_format );
        coeff += length;
        odd ^= length & 1;
    }
}    

/** This function implements an decimating FIR filter.
 *
 *  The function operates on a single set of input samples whose count is equal
 *  to the decimation factor. (i.e. and each call to the function processes
 *  ``decim_factor`` samples and results in one sample).
 * 
 *  The FIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``h[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[n]=x[n]*h[0]+x[n-1]*h[1]+x[n-2]*h[2]...+x[n-N+1]*h[N-1]
 * 
 *  ``filter_coeffs`` points to a coefficient array of size N = ``num_taps``.
 *  The filter coefficients
 *  are stored in forward order (e.g. ``h[0],h[1],h[N-1]``).
 * 
 *  The FIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in
 *  64-bit accumulater with the final result shifted to the required
 *  fixed-point format. Therefore overflow behavior of the 32-bit multiply
 *  operation and truncation behavior from final shifing of the accumulated
 *  multiplication results must be considered.
 * 
 *  \param  input_samples  The new samples to be decimated.
 *  \param  filter_coeffs  Pointer to FIR coefficients array arranged
 *                         as ``[b0,b1,b2,bN-1]``.
 *  \param  state_data     Pointer to filter state data array of length N.
 *                         Must be initialized at startup to all zero's.
 *  \param  tap_count      Filter tap count (N = tap_count = filter order + 1).
 *  \param  decim_factor   The decimation factor/index (i.e. the down-sampling ratio).
 *  \param  q_format       Fixed point format (i.e. number of fractional bits).
 *  \returns               The resulting decimated sample.
 */

int32_t lib_dsp_filters_decimate
(
    int32_t       input_samples[],
    const int32_t filter_coeffs[],
    int32_t       state_data[],
    int32_t       tap_count,
    int32_t       decim_factor,
    int32_t       q_format
) {
    int32_t  output;
    int32_t* dst = state_data + tap_count - 1;
    int32_t* src = dst - (decim_factor-1);
    
    /*
                                    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 bA bB
    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 * x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB -> y0
    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 *  ?  ?  ?  ?  ? x0 x1 x2 x3 x4 x5 x6 -> y0
    b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 *  ?  ?  ?  ?  ?  ?  ?  ?  ?  ? x0 x1 -> y0
    */

    output = lib_dsp_filters_fir( input_samples[0], filter_coeffs, state_data, tap_count, q_format );
    for( int32_t i = 0; i < tap_count - (decim_factor-1); ++i ) *dst-- = *src--;
    for( int32_t i = 0; i < decim_factor-1; ++i ) state_data[i] = input_samples[i+1];
    return output;    
}

/** This function implements a second order IIR (direct form I).
 *
 *  The function operates on a single sample of input and output data (i.e. and
 *  each call to the function processes one sample).
 * 
 *  The IIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``b[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[i]=x[n]*b[0]+x[n-1]*b[1]+x[n-2]*b2+x[n-1]*a[1]+x[n-2]*a[2]``
 * 
 *  The filter coefficients are stored in forward order (e.g. ``b0,b1,b2,a1,a2``).
 * 
 *  Example showing a single Biquad filter with samples and coefficients
 *  represented in Q28 fixed-point format:
 * 
 *  \code
 *  int32_t filter_coeff[5] = { Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1) };
 *  int32_t filter_state[4] = { 0, 0, 0, 0 };
 *  int32_t result = lib_dsp_biquad( sample, filter_coeff, filter_state, 28 );
 *  \endcode
 * 
 *  The IIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in
 *  64-bit accumulater with the final result shifted to the required fixed-point
 *  format. Therefore overflow behavior of the 32-bit multiply operation and
 *  truncation behavior from final shifing of the accumulated multiplication
 *  results must be considered.
 *
 *  \param  input_sample   The new sample to be processed.
 *  \param  filter_coeffs  Pointer to biquad coefficients array arranged as ``[b0,b1,b2,a1,a2]``.
 *  \param  state_data     Pointer to filter state data array (initialized at startup to zero's).
 *                         The length of the state data array is 4.
 *  \param  q_format       Fixed point format (i.e. number of fractional bits).
 *  \returns               The resulting filter output sample.
 */

int32_t lib_dsp_filters_biquad
(
    int32_t        input_sample,
    const int32_t* filter_coeffs,
    int32_t*       state_data,
    int32_t        q_format
) {
    uint32_t al; int32_t ah, c1,c2, s1,s2;
    asm("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(filter_coeffs));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(c1),"0"(0),"1"(1<<(q_format-1)));
    asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
    asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(c2),"0"(ah),"1"(al));
    asm("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(filter_coeffs));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(c1),"0"(ah),"1"(al));
    asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(c2),"0"(ah),"1"(al));
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(filter_coeffs[4]),"0"(ah),"1"(al));
    asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));    
    asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
    return ah;
}

/** This function implements a cascaded direct form I BiQuad filter
 *
 *  The function operates on a single sample of input and output data
 * (i.e. and each call to the function processes one sample).
 * 
 *  The IIR filter algorithm is based upon a sequence of multiply-accumulate
 *  (MAC) operations. Each filter coefficient ``b[i]`` is multiplied by a state
 *  variable which equals a previous input sample ``x[i]``, or
 *  ``y[n]=x[n]*b[0]+x[n-1]*b[1]+x[n-2]*b2+x[n-1]*a[1]+x[n-2]*a[2]``
 * 
 *  The filter coefficients are stored in forward order
 *  (e.g. ``section1:b0,b1,b2,a1,a2,sectionN:b0,b1,b2,a1,a2``).
 * 
 *  Example showing a 4x cascaded Biquad filter with samples and coefficients
 *  represented in Q28 fixed-point format:
 *   
 *  \code
 *  int32_t filter_coeff[20] = { Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
 *                           Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
 *                           Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1),
 *                           Q28(+0.5), Q(-0.1), Q28(-0.5), Q28(-0.1), Q28(0.1) };
 *  int32_t filter_state[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
 *  int32_t result = lib_dsp_cascaded_biquad( sample, filter_coeff, filter_state, 4, 28 );
 *  \endcode
 * 
 *  The IIR algorithm involves multiplication between 32-bit filter
 *  coefficients and 32-bit state data producing a 64-bit result for each
 *  coefficient and state data pair. Multiplication results are accumulated in
 *  64-bit accumulater with the final result shifted to the required fixed-point
 *  format. Therefore overflow behavior of the 32-bit multiply operation and
 *  truncation behavior from final shifing of the accumulated multiplication
 *  results must be considered.
 * 
 *  \param  input_sample   The new sample to be processed.
 *  \param  filter_coeffs  Pointer to biquad coefficients array for all BiQuad sections.
 *                         Arranged as ``[section1:b0,b1,b2,a1,a2,...sectionN:b0,b1,b2,a1,a2]``.
 *  \param  state_data     Pointer to filter state data array (initialized at startup to zero's).
 *                         The length of the state data array is ``num_sections`` * 4.
 *  \param  num_sections   Number of BiQuad sections.
 *  \param  q_format       Fixed point format (i.e. number of fractional bits).
 *  \returns               The resulting filter output sample.
 */

int32_t lib_dsp_filters_biquads
(
    int32_t        input_sample,
    const int32_t* filter_coeffs,
    int32_t*       state_data,
    int32_t        num_sections,
    int32_t        q_format
) {
    uint32_t al; int32_t ah, b0,b1, s1,s2;
    for( ;; )
    {
        switch( num_sections )
        {
            case 4:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[4]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[6]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[7]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[7]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;

            case 3:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[4]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(filter_coeffs[14]),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;

            case 2:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;
            
            case 1:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(filter_coeffs[4]),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));    
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            return ah;

            default:
            asm("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[0]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[2]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(input_sample),"r"(b0),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[4]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(state_data));
            input_sample = ah;
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(q_format-1)));
            asm("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("std %0,%1,%2[6]"::"r"(s1),"r"(input_sample),"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(filter_coeffs));
            asm("ldd %0,%1,%2[7]":"=r"(s2),"=r"(s1):"r"(state_data));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al));
            asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al));
            asm("lsats %0,%1,%2":"=r"(ah),"=r"(al):"r"(q_format),"0"(ah),"1"(al));
            asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(q_format));
            asm("std %0,%1,%2[7]"::"r"(s1),"r"(ah),"r"(state_data));
            num_sections -= 4; filter_coeffs += 20; state_data += 16;
            input_sample = ah;
            break;
        }
    }
    return 0;
}
