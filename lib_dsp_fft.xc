// Copyright (c) 2016, XMOS Ltd, All rights reserved
#include <xs1.h>
#include <xclib.h>
#include <stdint.h>
#include "lib_dsp_fft.h"

void lib_dsp_fft_bit_reverse( lib_dsp_fft_complex_t pts[], uint32_t N )
{
#if defined(__XS2A__)
    uint32_t shift = clz(N);
    for(uint32_t i = 1; i < N-1; i++) {
        uint32_t rev = bitrev(i) >> (shift + 1);
        if (rev > i) {
            uint32_t im1, re1, im2, re2;
            asm("ldd %0,%1,%2[%3]" : "=r"(im1), "=r" (re1): "r"(pts), "r" (i));
            asm("ldd %0,%1,%2[%3]" : "=r"(im2), "=r" (re2): "r"(pts), "r" (rev));
            asm("std %0,%1,%2[%3]" :: "r"(im1), "r" (re1), "r"(pts), "r" (rev));
            asm("std %0,%1,%2[%3]" :: "r"(im2), "r" (re2), "r"(pts), "r" (i));
        }
    }
#else
    //TODO
#endif
}

#pragma unsafe arrays
void lib_dsp_fft_forward_xs1 (
    lib_dsp_fft_complex_t pts[],
    uint32_t              N,
    const int32_t         sine[] )
{
    uint32_t shift = 30-clz(N);
    for(uint32_t step = 2 ; step <= N; step = step * 2, shift--) {
        uint32_t step2 = step >> 1;
        uint32_t step4 = step2 >> 1;
        uint32_t k;
        for(k = 0; k < step4 + (step2&1); k++) {
            int32_t rRe = sine[(N>>2)-(k<<shift)];
            int32_t rIm = sine[k<<shift];
            for(int32_t block = k+N-step; block >= 0; block-=step) {
                int32_t tRe = pts[block].re;
                int32_t tIm = pts[block].im;
                int32_t tRe2 = pts[block + step2].re;
                int32_t tIm2 = pts[block + step2].im;

                int32_t h;
                uint32_t l;
                int32_t sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);
                {h,l} = macs(tIm2, rIm, h, l);
                sRe2 = h;
                {h,l} = macs(tRe2, -rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h;
                tRe >>= 1;
                tIm >>= 1;
                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;

            }
        }
        for(k=(step2 & 1); k < step4; k++) {
            int32_t rRe = -sine[k<<shift];
            int32_t rIm = sine[(N>>2)-(k<<shift)];
            for(int32_t block = k+step4+N-step; block >= 0; block-=step) {
                int32_t tRe = pts[block].re;
                int32_t tIm = pts[block].im;
                int32_t tRe2 = pts[block + step2].re;
                int32_t tIm2 = pts[block + step2].im;

                int32_t h;
                uint32_t l;
                int32_t sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);
                {h,l} = macs(tIm2, rIm, h, l);
                sRe2 = h;
                {h,l} = macs(tRe2, -rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h;
                tRe >>= 1;
                tIm >>= 1;
                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;

            }
        }
    }
}

#pragma unsafe arrays
void lib_dsp_fft_inverse_xs1 (
    lib_dsp_fft_complex_t pts[],
    uint32_t              N,
    const int32_t         sine[] )
{
    uint32_t shift = 30-clz(N);
    for(uint32_t step = 2 ; step <= N; step = step * 2, shift--) {
        uint32_t step2 = step >> 1;
        uint32_t step4 = step2 >> 1;
        uint32_t k;
        for(k = 0; k < step4 + (step2&1); k++) {
            int32_t rRe = sine[(N>>2)-(k<<shift)];
            int32_t rIm = sine[k<<shift];
            for(int32_t block = k; block < k+N; block+=step) {
                int32_t tRe = pts[block].re;
                int32_t tIm = pts[block].im;
                int32_t tRe2 = pts[block + step2].re;
                int32_t tIm2 = pts[block + step2].im;
                int32_t h;
                uint32_t l;
                int32_t sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);  // Make this 0x40000000
                {h,l} = macs(tIm2, -rIm, h, l);
                sRe2 = h << 1;// | l >> 31;              // And include this part
                {h,l} = macs(tRe2, rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h << 1;// | l >> 31;
                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }
        for(k=(step2 & 1); k < step2-step4; k++) {
            int32_t rRe = -sine[k<<shift];
            int32_t rIm = sine[(N>>2)-(k<<shift)];
            for(int32_t block = k+step4; block < k+step4+N; block+=step) {
                int32_t tRe = pts[block].re;
                int32_t tIm = pts[block].im;
                int32_t tRe2 = pts[block + step2].re;
                int32_t tIm2 = pts[block + step2].im;
                int32_t h;
                uint32_t l;
                int32_t sRe2, sIm2;
                {h,l} = macs(tRe2, rRe, 0, 0x80000000);
                {h,l} = macs(tIm2, -rIm, h, l);
                sRe2 = h << 1;// | l >> 31;
                {h,l} = macs(tRe2, rIm, 0, 0x80000000);
                {h,l} = macs(tIm2, rRe, h, l);
                sIm2 = h << 1;// | l >> 31;
                pts[block].re = tRe + sRe2;
                pts[block].im = tIm + sIm2;
                pts[block+step2].re = tRe - sRe2;
                pts[block+step2].im = tIm - sIm2;
            }
        }
    }
}

#if defined(__XS2A__)

extern void lib_dsp_fft_forward_xs2 (
        lib_dsp_fft_complex_t pts[],
        uint32_t              N,
        const int32_t         sine[]);

extern void lib_dsp_fft_inverse_xs2 (
        lib_dsp_fft_complex_t pts[],
        uint32_t              N,
        const int32_t         sine[]);

extern void lib_dsp_fft_split_spectrum_xs2( lib_dsp_fft_complex_t pts[], uint32_t N );

extern void lib_dsp_fft_merge_spectra_xs2( lib_dsp_fft_complex_t pts[], uint32_t N );

extern void lib_dsp_fft_short_to_long_xs2( lib_dsp_fft_complex_t l[], lib_dsp_fft_complex_short_t s[], uint32_t N );

extern void lib_dsp_fft_long_to_short_xs2( lib_dsp_fft_complex_short_t s[], lib_dsp_fft_complex_t l[], uint32_t N );

#endif

void lib_dsp_fft_forward (
    lib_dsp_fft_complex_t pts[],
    uint32_t              N,
    const int32_t         sine[] ){
#if defined(__XS2A__)
    lib_dsp_fft_forward_xs2 (pts, N, sine);
#else
    lib_dsp_fft_forward_complex_xs1 (pts, N, sine);
#endif
}

void lib_dsp_fft_inverse (
    lib_dsp_fft_complex_t pts[],
    uint32_t              N,
    const int32_t         sine[] ){
#if defined(__XS2A__)
    lib_dsp_fft_inverse_xs2 (pts, N, sine);
#else
    lib_dsp_fft_inverse_xs1 (pts, N, sine);
#endif
}


void lib_dsp_fft_split_spectrum( lib_dsp_fft_complex_t pts[], uint32_t n ){
#if defined(__XS2A__)
    lib_dsp_fft_split_spectrum_xs2(pts, n);
#else
    for(uint32_t i=1;i<n/2;i++){
        int32_t a_re = (pts[i].re + pts[n-i].re)/2;
        int32_t a_im = (pts[i].im - pts[n-i].im)/2;
        int32_t b_re = (pts[i].im + pts[n-i].im)/2;
        int32_t b_im = (-pts[i].re + pts[n-i].re)/2;

        pts[i].re = a_re;
        pts[i].im = a_im;
        pts[n-i].re = b_re;
        pts[n-i].im = b_im;
    }

    int32_t re = pts[n/2].re;
    int32_t im = pts[n/2].im;

    pts[n/2].re = pts[0].im;
    pts[n/2].im = im;
    pts[0].im = re;


    for(uint32_t i=1;i<n/4;i++){
        lib_dsp_fft_complex_t a = pts[n/2 + i];
        lib_dsp_fft_complex_t b = pts[n - i];
        pts[n/2 + i] = b;
        pts[n - i]  = a;
    }
#endif
}

void lib_dsp_fft_merge_spectra( lib_dsp_fft_complex_t pts[], uint32_t n ){
#if defined(__XS2A__)
    lib_dsp_fft_merge_spectra_xs2(pts, n);
#else
    for(uint32_t i=1;i<n/4;i++){
        lib_dsp_fft_complex_t a = pts[n/2 + i];
        lib_dsp_fft_complex_t b = pts[n - i];
        pts[n/2 + i] = b;
        pts[n - i]  = a;
    }

    int32_t t = pts[0].im;
    pts[0].im = pts[n/2].re;
    pts[n/2].re = t;

    for(uint32_t i=1;i<n/2;i++){

        int32_t a_re = pts[i].re;
        int32_t a_im = pts[i].im;
        int32_t b_re = pts[n-i].re;
        int32_t b_im = pts[n-i].im;

        pts[i].re = a_re-b_im;
        pts[i].im = a_im+b_re;
        pts[n-i].re = b_im+a_re;
        pts[n-i].im = b_re-a_im;
    }
#endif
}

void lib_dsp_fft_short_to_long( lib_dsp_fft_complex_t l[], lib_dsp_fft_complex_short_t s[], uint32_t N ){
#if defined(__XS2A__)
    lib_dsp_fft_short_to_long_xs2(l, s, N);
#else
    for(uint32_t i=0;i<n;i++){
        l[i].re = ((int)s[i].re)<<16;
        l[i].im = ((int)s[i].im)<<16;
    }
#endif
}

void lib_dsp_fft_long_to_short( lib_dsp_fft_complex_short_t s[], lib_dsp_fft_complex_t l[], uint32_t N ){
#if defined(__XS2A__)
    lib_dsp_fft_long_to_short_xs2(s, l, N);
#else
    for(uint32_t i=0;i<n;i++){
        s[i].re = l[i].re>>16;
        s[i].im = l[i].im>>16;
    }
#endif
}

