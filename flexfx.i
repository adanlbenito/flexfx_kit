#include "flexfx.h"

#define _fir_norm( nn ) \
\
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(c1),"=r"(c0):"r"(cc),"r"(2*nn+0)); \
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(s2),"=r"(s1):"r"(ss),"r"(2*nn+0)); \
    asm volatile("std %0,%1,%2[%3]"::"r" (s1), "r"(s0),"r"(ss),"r"(2*nn+0)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(c1),"=r"(c0):"r"(cc),"r"(2*nn+1)); \
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(s0),"=r"(s3):"r"(ss),"r"(2*nn+1)); \
    asm volatile("std %0,%1,%2[%3]" ::"r"(s3), "r"(s2),"r"(ss),"r"(2*nn+1)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s2),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s3),"0"(ah),"1"(al));

#define _fir_norm0() \
\
    asm volatile("ldd %0,%1,%2[0]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[0]"  :"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[0]" ::"r" (s1), "r"(s0),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[1]"  :"=r"(s0),"=r"(s3):"r"(ss)); \
    asm volatile("std %0,%1,%2[1]"  ::"r"(s3), "r"(s2),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s2),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s3),"0"(ah),"1"(al));

#define _fir_norm1() \
\
    asm volatile("ldd %0,%1,%2[2]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[2]"  :"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[2]" ::"r" (s1), "r"(s0),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[3]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[3]"  :"=r"(s0),"=r"(s3):"r"(ss)); \
    asm volatile("std %0,%1,%2[3]"  ::"r"(s3), "r"(s2),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s2),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s3),"0"(ah),"1"(al));

#define _fir_norm2() \
\
    asm volatile("ldd %0,%1,%2[4]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[4]"  :"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[4]" ::"r" (s1), "r"(s0),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[5]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[5]"  :"=r"(s0),"=r"(s3):"r"(ss)); \
    asm volatile("std %0,%1,%2[5]"  ::"r"(s3), "r"(s2),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s2),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s3),"0"(ah),"1"(al));

#define _fir_norm3() \
\
    asm volatile("ldd %0,%1,%2[6]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[6]"  :"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[6]" ::"r" (s1), "r"(s0),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[7]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[7]"  :"=r"(s0),"=r"(s3):"r"(ss)); \
    asm volatile("std %0,%1,%2[7]"  ::"r"(s3), "r"(s2),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s2),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s3),"0"(ah),"1"(al));

#define _fir_norm4() \
\
    asm volatile("ldd %0,%1,%2[8]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[8]"  :"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[8]" ::"r" (s1), "r"(s0),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[9]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[9]"  :"=r"(s0),"=r"(s3):"r"(ss)); \
    asm volatile("std %0,%1,%2[9]"  ::"r"(s3), "r"(s2),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s2),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s3),"0"(ah),"1"(al));

#define _fir_norm5() \
\
    asm volatile("ldd %0,%1,%2[10]" :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[10]" :"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[10]"::"r" (s1), "r"(s0),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[11]" :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[11]" :"=r"(s0),"=r"(s3):"r"(ss)); \
    asm volatile("std %0,%1,%2[11]" ::"r"(s3), "r"(s2),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s2),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s3),"0"(ah),"1"(al));

#define _fir_read( nn ) \
\
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(c1),"=r"(c0):"r"(cc),"r"(2*nn+0)); \
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(s1),"=r"(s0):"r"(ss),"r"(2*nn+0)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(c1),"=r"(c0):"r"(cc),"r"(2*nn+1)); \
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(s1),"=r"(s0):"r"(ss),"r"(2*nn+1)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_read0() \
\
    asm volatile("ldd %0,%1,%2[0]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[0]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[1]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_read1() \
\
    asm volatile("ldd %0,%1,%2[2]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[2]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[3]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[3]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_read2() \
\
    asm volatile("ldd %0,%1,%2[4]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[4]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[5]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[5]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_read3() \
\
    asm volatile("ldd %0,%1,%2[6]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[6]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[7]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[7]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_read4() \
\
    asm volatile("ldd %0,%1,%2[8]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[8]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[9]"  :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[9]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_read5() \
\
    asm volatile("ldd %0,%1,%2[10]" :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[10]" :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[11]" :"=r"(c1),"=r"(c0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[11]" :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_step( nn, ii, rr ) \
\
    c0 = cc[(4*nn+0)*rr+ii]; c1 = cc[(4*nn+1)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(s1),"=r"(s0):"r"(ss),"r"(2*nn+0)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    c0 = cc[(4*nn+2)*rr+ii]; c1 = cc[(4*nn+3)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[%3]" :"=r"(s1),"=r"(s0):"r"(ss),"r"(2*nn+1)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_step0( ii, rr ) \
\
    c0 = cc[(4*0+0)*rr+ii]; c1 = cc[(4*0+1)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[0]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    c0 = cc[(4*0+2)*rr+ii]; c1 = cc[(4*0+3)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[1]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_step1( ii, rr ) \
\
    c0 = cc[(4*1+0)*rr+ii]; c1 = cc[(4*1+1)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[2]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    c0 = cc[(4*1+2)*rr+ii]; c1 = cc[(4*1+3)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[3]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_step2( ii, rr ) \
\
    c0 = cc[(4*2+0)*rr+ii]; c1 = cc[(4*2+1)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[4]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    c0 = cc[(4*2+2)*rr+ii]; c1 = cc[(4*2+3)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[5]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_step3( ii, rr ) \
\
    c0 = cc[(4*3+0)*rr+ii]; c1 = cc[(4*3+1)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[6]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    c0 = cc[(4*3+2)*rr+ii]; c1 = cc[(4*3+3)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[7]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_step4( ii, rr ) \
\
    c0 = cc[(4*4+0)*rr+ii]; c1 = cc[(4*4+1)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[8]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    c0 = cc[(4*4+2)*rr+ii]; c1 = cc[(4*4+3)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[9]"  :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_step5( ii, rr ) \
\
    c0 = cc[(4*5+0)*rr+ii]; c1 = cc[(4*5+1)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[10]" :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al)); \
    c0 = cc[(4*5+2)*rr+ii]; c1 = cc[(4*5+3)*rr+ii]; \
    asm volatile("ldd %0,%1,%2[11]" :"=r"(s1),"=r"(s0):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c0),"r"(s0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(ah),"1"(al));

#define _fir_step_tt( tt_, ii, rr ) \
{ \
    int tt = tt_; while( tt >= 24 ) { \
        _fir_step0(ii,rr); _fir_step1(ii,rr); _fir_step2(ii,rr); \
        _fir_step3(ii,rr); _fir_step4(ii,rr); _fir_step5(ii,rr); \
        cc += 24; ss += 24; tt -= 24; \
    } \
    switch( tt ) { \
        case 20: _fir_step0(ii,rr); _fir_step1(ii,rr); _fir_step2(ii,rr); \
                 _fir_step3(ii,rr); _fir_step4(ii,rr); break; \
        case 16: _fir_step0(ii,rr); _fir_step1(ii,rr); _fir_step2(ii,rr); \
                 _fir_step3(ii,rr); break; \
        case 12: _fir_step0(ii,rr); _fir_step1(ii,rr); _fir_step2(ii,rr); break; \
        case  8: _fir_step0(ii,rr); _fir_step1(ii,rr); break; \
        case  4: _fir_step0(ii,rr); break; \
    } \
}

static inline int _dsp_fir( int xx, const int* cc, int* ss, int nn )
{
    int c0, c1, s0 = xx, s1, s2, s3, ah = 0; unsigned al = 1<<(QQ-1);
    while( nn >= 24 ) {
        _fir_norm0(); _fir_norm1(); _fir_norm2(); _fir_norm3(); _fir_norm4(); _fir_norm5();
        cc += 24; ss += 24; nn -= 24;
    }
    switch( nn ) {
        case 20: _fir_norm0(); _fir_norm1(); _fir_norm2(); _fir_norm3(); _fir_norm4(); break;
        case 16: _fir_norm0(); _fir_norm1(); _fir_norm2(); _fir_norm3(); break;
        case 12: _fir_norm0(); _fir_norm1(); _fir_norm2(); break;
        case  8: _fir_norm0(); _fir_norm1(); break;
        case  4: _fir_norm0(); break;
    }
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ));
    return ah;
}

static inline void _dsp_upsample( int* xx, const int* cc_, int* ss_, int nn, int rr )
{
    const int *cc = cc_; int *ss = ss_;
    int c0, c1, s0, s1, ah; unsigned al;
    memmove( ss+1, ss, 4*(nn/rr-1) ); ss[0] = xx[0];
    switch( rr ) {
        case 3:
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/3,0,3);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(FQ(3.0)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/3,1,3);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[1]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[1]),"r"(FQ(3.0)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[1]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/3,2,3);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(FQ(3.0)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
        break;
        case 5:
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/5,0,5);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[4]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[4]),"r"(FQ(5.0)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[4]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/5,1,5);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[3]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[3]),"r"(FQ(5.0)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[3]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/5,2,5);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(FQ(5.0)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/5,3,5);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[1]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[1]),"r"(FQ(5.0)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[1]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/5,4,5);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(FQ(5.0)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
        break;
        case 6:
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/6,0,6);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[5]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[5]),"r"(FQ(6)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[5]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/6,1,6);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[4]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[4]),"r"(FQ(6)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[4]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/6,2,6);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[3]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[3]),"r"(FQ(6)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[3]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/6,3,6);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(FQ(6)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/6,4,6);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[1]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[1]),"r"(FQ(6)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[1]):"r"(ah),"r"(al),"r"(QQ));
        ah = 0; al = 1<<(QQ-1); _fir_step_tt(nn/6,5,6);
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(FQ(6)),"0"(0),"1"(1<<(QQ-1))); \
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
        break;
    }
}

static inline void _dsp_dnsample( int* xx, const int* cc, int* ss, int nn, int rr )
{
    int c0, c1, s0, s1, ah = 0; unsigned al = 1<<(QQ-1);
    memmove( ss+rr, ss, 4*(nn-rr) );
    switch( rr ) {
        case 2: asm volatile("std %0,%1,%2[0]"::"r"(xx[1]),"r"(xx[0]),"r"(ss)); break;
        case 3: asm volatile("std %0,%1,%2[0]"::"r"(xx[1]),"r"(xx[0]),"r"(ss)); ss[2]=xx[2]; break;
        case 4: asm volatile("std %0,%1,%2[0]"::"r"(xx[1]),"r"(xx[0]),"r"(ss));
                asm volatile("std %0,%1,%2[1]"::"r"(xx[3]),"r"(xx[2]),"r"(ss)); break;
        case 5: asm volatile("std %0,%1,%2[0]"::"r"(xx[1]),"r"(xx[0]),"r"(ss));
                asm volatile("std %0,%1,%2[1]"::"r"(xx[3]),"r"(xx[2]),"r"(ss)); ss[4]=xx[4]; break;
        case 6: asm volatile("std %0,%1,%2[0]"::"r"(xx[1]),"r"(xx[0]),"r"(ss));
                asm volatile("std %0,%1,%2[1]"::"r"(xx[3]),"r"(xx[2]),"r"(ss));
                asm volatile("std %0,%1,%2[2]"::"r"(xx[5]),"r"(xx[4]),"r"(ss)); break;
        case 7: asm volatile("std %0,%1,%2[0]"::"r"(xx[1]),"r"(xx[0]),"r"(ss));
                asm volatile("std %0,%1,%2[1]"::"r"(xx[3]),"r"(xx[2]),"r"(ss));
                asm volatile("std %0,%1,%2[2]"::"r"(xx[5]),"r"(xx[4]),"r"(ss)); ss[6]=xx[6]; break;
        case 8: asm volatile("std %0,%1,%2[0]"::"r"(xx[1]),"r"(xx[0]),"r"(ss));
                asm volatile("std %0,%1,%2[1]"::"r"(xx[3]),"r"(xx[2]),"r"(ss));
                asm volatile("std %0,%1,%2[2]"::"r"(xx[5]),"r"(xx[4]),"r"(ss));
                asm volatile("std %0,%1,%2[3]"::"r"(xx[7]),"r"(xx[6]),"r"(ss)); break;
    }
    while( nn >= 24 ) {
        _fir_read0(); _fir_read1(); _fir_read2(); _fir_read3(); _fir_read4(); _fir_read5();
        cc += 24; ss += 24; nn -= 24;
    }
    switch( nn ) {
        case 20: _fir_read0(); _fir_read1(); _fir_read2(); _fir_read3(); _fir_read4(); break;
        case 16: _fir_read0(); _fir_read1(); _fir_read2(); _fir_read3(); break;
        case 12: _fir_read0(); _fir_read1(); _fir_read2(); break;
        case  8: _fir_read0(); _fir_read1(); break;
        case  4: _fir_read0(); break;
    }
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
}

#define _dsp_iir_1( xx, cc, ss ) \
{ \
    unsigned al; int ah, c1,c2, s1,s2; \
    asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(c1),"0"(0),"1"(0)); \
    asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[0]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(c2),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(c1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(c2),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[2]":"=r"(c2),"=r"(c1):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(c1),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
}

#define _dsp_iir_2( xx, cc, ss ) \
{ \
    unsigned al; int ah, b0,b1, s1,s2; \
    asm volatile("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(b0),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[0]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[2]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
}

#define _dsp_iir_3( xx, cc, ss ) \
{ \
    unsigned al; int ah, b0,b1, s1,s2; \
    asm volatile("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(b0),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[0]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[2]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
    asm volatile("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(b0),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[4]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(cc[14]),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
}

#define _dsp_iir_4( xx, cc, ss ) \
{ \
    unsigned al; int ah, b0,b1, s1,s2; \
    asm volatile("ldd %0,%1,%2[0]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(b0),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[0]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[2]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[1]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("ldd %0,%1,%2[3]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[2]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[2]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[4]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[3]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[3]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
    asm volatile("ldd %0,%1,%2[5]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(b0),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("ldd %0,%1,%2[4]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[4]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[6]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[5]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[7]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[5]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(ah),"r"(b1),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("ldd %0,%1,%2[8]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[6]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("std %0,%1,%2[6]"::"r"(s1),"r"(xx),"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("ldd %0,%1,%2[9]":"=r"(b1),"=r"(b0):"r"(cc)); \
    asm volatile("ldd %0,%1,%2[7]":"=r"(s2),"=r"(s1):"r"(ss)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s1),"r"(b0),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(s2),"r"(b1),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("std %0,%1,%2[7]"::"r"(s1),"r"(ah),"r"(ss)); \
    xx = ah; \
}

static inline void math_sum_X1z( const int* xx, int zz, int nn ) // r = X[0:N-1] * 1.0 + z
{
    unsigned al = 0; int ah = 0, c1,c2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ));
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        break;
        case 1:
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(zz),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
        break;
    }
}

static inline int _math_sqr_x( int xx ) // r = xx^0.5
{
    return 0;
}

static inline int _math_min_X( const int* xx, int nn ) // r = min(X[0:N-1])
{
    int c1,c2, yy = 0;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < yy ) yy = c1; if( c2 < yy ) yy = c2;
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < yy ) yy = c1; if( c2 < yy ) yy = c2;
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < yy ) yy = c1; if( c2 < yy ) yy = c2;
        if( xx[2] < yy ) yy = xx[2];
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < yy ) yy = c1; if( c2 < yy ) yy = c2;
        break;
        case 1: if( xx[2] < yy ) yy = xx[2];
        break;
    }
    return yy;
}

static inline int _math_max_X( const int* xx, int nn ) // r = max(X[0:N-1])
{
    int c1,c2, yy = 0;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 > yy ) yy = c1; if( c2 > yy ) yy = c2;
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 > yy ) yy = c1; if( c2 > yy ) yy = c2;
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 > yy ) yy = c1; if( c2 > yy ) yy = c2;
        if( xx[2] > yy ) yy = xx[2];
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 > yy ) yy = c1; if( c2 > yy ) yy = c2;
        break;
        case 1: if( xx[2] > yy ) yy = xx[2];
        break;
    }
    return yy;
}

static inline int _math_avg_X( const int* xx, int nn ) // r = mean(X[0:N-1])
{
    return 0;
}

static inline void _math_sum_X( const int* xx, int nn, int* ah_, unsigned* al_ ) // r = sum(X[0:N-1])
{
    unsigned al = 0; int ah = 0, c1,c2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        break;
        case 1:
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        break;
    }
    *ah_ = ah; *al_ = al;
}

static inline void _math_pwr_X( const int* xx, int nn, int* ah_, unsigned* al_ ) // r = sum(X[0:N-1]*X[0:N-1])
{
    unsigned al = 0; int ah = 0, c1,c2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(c1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(c2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(c1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(c2),"0"(0),"1"(1<<(QQ-1)));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(c1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(c2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(xx[2]),"0"(0),"1"(1<<(QQ-1)));
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(c1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(c2),"0"(0),"1"(1<<(QQ-1)));
        break;
        case 1:
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(xx[0]),"0"(0),"1"(1<<(QQ-1)));
        break;
    }
    *ah_ = ah; *al_ = al;
}

static inline int _math_rms_X( const int* xx, int nn ) // r = sum(X[0:N-1]*X[0:N-1]) ^ 0.5
{
    unsigned al; int ah;
    _math_pwr_X( xx, nn, &ah, &al );
    // <TODO> ah:al ^ 0.5
    return 0;
}

static inline void _math_asm_X( const int* xx, int nn, int* ah_, unsigned* al_ ) // r = sum(abs(X[0:N-1))
{
    unsigned al = 0; int ah = 0, c1,c2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < 0 ) c1 = -c1; if( c2 < 0 ) c2 = -c2;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < 0 ) c1 = -c1; if( c2 < 0 ) c2 = -c2;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < 0 ) c1 = -c1; if( c2 < 0 ) c2 = -c2;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        if( xx[2] > 0 ) asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(+1),"0"(0),"1"(1<<(QQ-1)));
        else            asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(-1),"0"(0),"1"(1<<(QQ-1)));
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < 0 ) c1 = -c1; if( c2 < 0 ) c2 = -c2;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(1),"0"(0),"1"(1<<(QQ-1)));
        break;
        case 1:
        if( xx[2] > 0 ) asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(+1),"0"(0),"1"(1<<(QQ-1)));
        else            asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(-1),"0"(0),"1"(1<<(QQ-1)));
        break;
    }
    *ah_ = ah; *al_ = al;
}

static inline void _math_abs_X( int* xx, int nn ) // X[0:N-1] = abs(X[0:N-1])
{
    int c1,c2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < 0 ) c1 = -c1; if( c2 < 0 ) c2 = -c2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < 0 ) c1 = -c1; if( c2 < 0 ) c2 = -c2;
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < 0 ) c1 = -c1; if( c2 < 0 ) c2 = -c2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        if( xx[2] < 0 ) xx[2] = -xx[2];
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        if( c1 < 0 ) c1 = -c1; if( c2 < 0 ) c2 = -c2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        break;
        case 1: if( xx[2] < 0 ) xx[0] = -xx[0];
        break;
    }
}

static inline void _math_sqr_X( int* xx, int nn ) // X[0:N-1] = X[0:N-1]^0.5
{
}

static inline void _math_mac_X1z( int* xx, int zz, int nn ) // X[0:N-1] = X[0:N-1] + zz
{
    int c1,c2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx)); c1 += zz; c2 += zz;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx)); c1 += zz; c2 += zz;
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx)); c1 += zz; c2 += zz;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        xx[2] += zz;
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx)); c1 += zz; c2 += zz;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        break;
        case 1: xx[0] += zz;
        break;
    }
}

static inline void _math_mac_X1Z( int* xx, const int* zz, int nn ) // X[0:N-1] = X[0:N-1] + zz[0:N-1]
{
    int c1,c2, s1,s2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(zz));
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx)); c1 += s1; c2 += s2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(zz));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx)); c1 += s2; c2 += s2;
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(zz));
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx)); c1 += s1; c2 += s2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        xx[2] += zz[2];
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(zz));
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx)); c1 += s1; c2 += s2;
        break;
        case 1: xx[0] += zz[0];
        break;
    }
}

static inline void _math_mac_Xy0( int* xx, int yy, int nn ) // X[0:N-1] = X[0:N-1] * y
{
    unsigned al; int ah, c1,c2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ));
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        break;
        case 1:
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
        break;
    }
}

static inline void _math_mac_XY0( int* xx, const int* yy, int nn ) // X[0:N-1] = X[0:N-1] * Y[0:N-1]
{
    unsigned al; int ah, c1,c2, s1,s2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(yy[2]),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ));
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ));
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        break;
        case 1:
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(yy[0]),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ));
        break;
    }
}

static inline void _math_mac_Xyz( int* xx, int yy, int zz, int nn ) // X[] = X[] * y + z
{
    unsigned al; int ah, c1,c2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += zz;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += zz;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += zz;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += zz;
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += zz;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += zz;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ)); xx[2] += zz;
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += zz;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += zz;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        break;
        case 1:
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ)); xx[0] += zz;
        break;
    }
}

static inline void _math_mac_XyZ( int* xx, int yy, const int* zz, int nn ) // X[] = X[] * y + Z[]
{
    unsigned al; int ah, c1,c2, s1, s2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += s1;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += s2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += s1;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += s2;
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += s1;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += s2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ)); xx[2] += zz[2];
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += s1;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += s2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        break;
        case 1:
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(yy),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ)); xx[0] += zz[0];
        break;
    }
}

static inline void _math_mac_XYz( int* xx, const int* yy, int zz, int nn ) // X[] = X[] * Y[] + z
{
    unsigned al; int ah, c1,c2, s1,s2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += zz;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += zz;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += zz;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += zz;
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += zz;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += zz;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(yy[2]),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ)); xx[2] += zz;
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += zz;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += zz;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        break;
        case 1:
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(yy[0]),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ)); xx[0] += zz;
        break;
    }
}

static inline void _math_mac_XYZ( int* xx, const int* yy, const int* zz, int nn ) // X[] = X[] * Y[] + Z[]
{
    unsigned al; int ah, c1,c2, s1,s2, h1,h2;
    while( nn >= 4 ) {
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("ldd %0,%1,%2[0]":"=r"(h2),"=r"(h1):"r"(zz));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += h1;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += h2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[1]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("ldd %0,%1,%2[1]":"=r"(h2),"=r"(h1):"r"(zz));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += h1;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += h2;
        asm volatile("std %0,%1,%2[1]"::"r"(c2), "r"(c1),"r"(xx));
        xx += 4; nn -= 4;
    }
    switch( nn ) {
        case 3:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("ldd %0,%1,%2[0]":"=r"(h2),"=r"(h1):"r"(zz));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += h1;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += h2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[2]),"r"(yy[2]),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[2]):"r"(ah),"r"(al),"r"(QQ)); xx[2] += zz[2];
        break;
        case 2:
        asm volatile("ldd %0,%1,%2[0]":"=r"(c2),"=r"(c1):"r"(xx));
        asm volatile("ldd %0,%1,%2[0]":"=r"(s2),"=r"(s1):"r"(yy));
        asm volatile("ldd %0,%1,%2[0]":"=r"(h2),"=r"(h1):"r"(zz));
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c1),"r"(s1),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c1):"r"(ah),"r"(al),"r"(QQ)); c1 += h1;
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(c2),"r"(s2),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(c2):"r"(ah),"r"(al),"r"(QQ)); c2 += h2;
        asm volatile("std %0,%1,%2[0]"::"r"(c2), "r"(c1),"r"(xx));
        break;
        case 1:
        asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx[0]),"r"(yy[0]),"0"(0),"1"(1<<(QQ-1)));
        asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx[0]):"r"(ah),"r"(al),"r"(QQ)); xx[0] += zz[0];
        break;
    }
}

// 0 (100% dry) <= MM <= 1 (100% wet)
#define _dsp_blend( xx, dry, wet, blend ) \
{ \
    int ah = 0; unsigned al = 1<<(QQ-1); \
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(dry),"r"(FQ(1.0)-blend/2),"0"(ah),"1"(al) ); \
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(wet),"r"(blend/2),"0"(ah),"1"(al) ); \
    asm("lextract %0,%1,%2,%3,32":"=r"(xx):"r"(ah),"r"(al),"r"(QQ)); \
}

// Envelope detector (digital RC circuit, wp279.pdf): y[n] = y[n–1] + k * (x[n] – y[n–1])
#define _dsp_envelope( yy, xx, kk, ss ) \
{ \
    int ah; unsigned al; \
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(FQ(1.0)),"r"(ss[0]),"0"(0),"1"(1<<(QQ-1))); \
    if( xx >= 0 ) asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(kk),"r"(+xx-ss[0]),"0"(ah),"1"(al)); \
    else          asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(kk),"r"(-xx-ss[0]),"0"(ah),"1"(al)); \
    asm("lextract %0,%1,%2,%3,32":"=r"(ss[0]):"r"(ah),"r"(al),"r"(QQ)); \
    yy = ss[0]; \
}

// Differentiator: y[n]=x[n] - x[n-1]
// Leaky Integrator: y[n]=pole*y[n-1] + x[n]
// y[n] = x[n] - x[n-1] + R*y[n-1]

#define _dsp_dcblock( xx, kk, ss ) \
{ \
    int ah; unsigned al; \
    asm("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(kk),"r"(ss[1]),"0"(0),"1"(1<<(QQ-1))); \
    asm("lextract %0,%1,%2,%3,32":"=r"(ah):"r"(ah),"r"(al),"r"(QQ)); \
    ss[1] = xx - ss[0] + ah; ss[0] = xx; xx = ss[1]; \
}

#define _dsp_slewlimit( xx, kk, ss ) \
{ \
    if( xx > ss[1]+kk) xx = ss[1]+kk; if( xx < ss[1]-kk ) xx = ss[1]-kk; ss[1] = xx; \
}

//     y1    z     y2          y3      y1 <= z <= y2
// ----+-----+-----+-----------+--
//     0     x     1           2       0.0 <= x < 1.0
//
// z1 = y1 * ((x-1)*(x-2))/((0-1)*(0-2)) = y1 * ((x^2-3x+2)/+2)
// z2 = y2 * ((x-0)*(x-2))/((1-0)*(1-2)) = y2 * ((x^2-2x+0)/-1)
// z3 = y3 * ((x-0)*(x-1))/((2-0)*(2-1)) = y3 * ((x^2-1x+0)/+2)
//
// x1 = ff - FQ(0.0), x2 = ff - FQ(1.0), x3 = ff - FQ(2.0)
// z1 = y1 * x1 * x2 / +2
// z2 = y2 * x0 * x2 / -1
// z3 = y3 * x0 * x1 / +2
// xx = z1 + z2 + x3

#define _dsp_interp( xx, ff, y1, y2 ) \
{ \
    int ah; unsigned al; \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(y1),"r"(FQ(1.0)-ff),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(y2),"r"(ff),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx):"r"(ah),"r"(al),"r"(QQ)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(xx),"r"(FQ(0.9)),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx):"r"(ah),"r"(al),"r"(QQ)); \
}

#define _dsp_lagrange( xx, ff, y1, y2, y3 ) \
{ \
    int z1, z2, z3, ah; unsigned al; \
    int x0 = ff - FQ(0.0), x1 = ff - FQ(1.0), x2 = ff - FQ(2.0); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x1),"r"(x2),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(z1):"r"(ah),"r"(al),"r"((QQ+1))); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(-x2),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(z2):"r"(ah),"r"(al),"r"((QQ+0))); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(x0),"r"(x1),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(z3):"r"(ah),"r"(al),"r"((QQ+1))); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(y1),"r"(z1),"0"(0),"1"(1<<(QQ-1))); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(y2),"r"(z2),"0"(ah),"1"(al)); \
    asm volatile("maccs %0,%1,%2,%3":"=r"(ah),"=r"(al):"r"(y3),"r"(z3),"0"(ah),"1"(al)); \
    asm volatile("lextract %0,%1,%2,%3,32":"=r"(xx):"r"(ah),"r"(al),"r"(QQ)); \
}
