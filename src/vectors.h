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

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
    typedef __vector unsigned __int128 vui128_t;
#else
    typedef __vector unsigned int vui128_t;
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define CONST_VINT128(__dw0, __dw1) (vui128_t){__dw1, __dw0}
    #define VEC_DW_H 1
    #define VEC_DW_L 0
    #define VEC_W_H 3
    #define VEC_W_L 0
    #define VEC_HW_H 7
    #define VEC_BYTE_L 0
    #define VEC_BYTE_H 15
    #define VEC_BYTE_HHW 14
    #define VPERM_0_MASK_H 0x08090A0B0C0D0E0F
    #define VPERM_0_MASK_L 0x0001020304050607
    #define VPERM_1_MASK_H 0x18191A1B1C1D1E1F
    #define VPERM_1_MASK_L 0x1011121314151617


#else
    #define CONST_VINT128(__dw0, __dw1) (vui128_t){__dw0, __dw1}
    #define VEC_DW_H 0
    #define VEC_DW_L 1
    #define VEC_W_H 0
    #define VEC_W_L 3
    #define VEC_HW_H 0
    #define VEC_BYTE_L 15
    #define VEC_BYTE_H 0
    #define VEC_BYTE_HHW 1
    #define VPERM_0_MASK_H 0x0001020304050607
    #define VPERM_0_MASK_L 0x08090A0B0C0D0E0F
    #define VPERM_1_MASK_H 0x1011121314151617
    #define VPERM_1_MASK_L 0x18191A1B1C1D1E1F
#endif

typedef union {
    vui128_t vx1;

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
    printf(" < [%lu, %lu] > \n", c.vx1[VEC_DW_H], c.vx1[VEC_DW_L]);
}

#endif
