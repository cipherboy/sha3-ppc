/*
 * Copyright (C) 2016 Alexander Scheel
 *
 * Licensed under the BSD 2-Clause, MIT, and Apache v2 Public Licenses.
 */

#include <stdint.h>
#include <stddef.h>
#include <altivec.h>

#pragma once
#ifndef SHA3_VECTORS_H
#define SHA3_VECTORS_H


typedef __vector unsigned long long vui64_t;

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
    typedef __vector unsigned __int128 vui128_t;
#else
    typedef __vector unsigned int vui128_t;
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define CONST_VINT64_DW(__dw0, __dw1) {__dw1, __dw0}
    #define CONST_VINT128_DW(__dw0, __dw1) (vui64_t){__dw1, __dw0}
    #define CONST_VINT128(__dw0, __dw1) (vui128_t)((vui64_t){__dw1, __dw0})
    #define CONST_VINT128_W(__w0, __w1, __w2, __w3) (vui32_t){__w3, __w2, __w1, __w0}
    #define CONST_VINT32_W(__w0, __w1, __w2, __w3) {__w3, __w2, __w1, __w0}
    #define VEC_DW_H 1
    #define VEC_DW_L 0
    #define VEC_W_H 3
    #define VEC_W_L 0
    #define VEC_HW_H 7
    #define VEC_BYTE_L 0
    #define VEC_BYTE_H 15
    #define VEC_BYTE_HHW 14
    #define VEC_PERM_0_MASK_H 0x08090A0B0C0D0E0FULL
    #define VEC_PERM_0_MASK_L 0x0001020304050607ULL
    #define VEC_PERM_1_MASK_H 0x18191A1B1C1D1E1FULL
    #define VEC_PERM_1_MASK_L 0x1011121314151617ULL
#else
    #define CONST_VINT64_DW(__dw0, __dw1) {__dw0, __dw1}
    #define CONST_VINT128_DW(__dw0, __dw1) (vui64_t){__dw0, __dw1}
    #define CONST_VINT128(__dw0, __dw1) (vui128_t)((vui64_t){__dw0, __dw1})
    #define CONST_VINT128_W(__w0, __w1, __w2, __w3) (vui32_t){__w0, __w1, __w2, __w3}
    #define CONST_VINT32_W(__w0, __w1, __w2, __w3) {__w0, __w1, __w2, __w3}
    #define VEC_DW_H 0
    #define VEC_DW_L 1
    #define VEC_W_H 0
    #define VEC_W_L 3
    #define VEC_HW_H 0
    #define VEC_BYTE_L 15
    #define VEC_BYTE_H 0
    #define VEC_BYTE_HHW 1
    #define VEC_PERM_0_MASK_H 0x0001020304050607ULL
    #define VEC_PERM_0_MASK_L 0x08090A0B0C0D0E0FULL
    #define VEC_PERM_1_MASK_H 0x1011121314151617ULL
    #define VEC_PERM_1_MASK_L 0x18191A1B1C1D1E1FULL
#endif

typedef union {
    vui128_t vx;

    struct {
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            uint64_t lower;
            uint64_t upper;
        #else
            uint64_t upper;
            uint64_t lower;
        #endif
    } ord;
} p_v128;

static inline void print_vec(p_v128 c)
{
    printf(" < [%lu, %lu] > \n", c.vx[VEC_DW_H], c.vx[VEC_DW_L]);
}

#endif
