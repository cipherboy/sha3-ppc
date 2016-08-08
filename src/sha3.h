/*
 * Copyright (C) 2016 Alexander Scheel
 *
 * Licensed under the BSD 2-Clause, MIT, and Apache v2 Public Licenses.
 */

#include <stdio.h>
#include "vectors.h"

#pragma once
#ifndef SHA3_SHA3_H
#define SHA3_SHA3_H

typedef union {
    struct {
        p_v128 a;
        p_v128 b;
        p_v128 c;
    } vec;
    uint64_t arr[5];
} sha3_sheet;

// Vector slot (5 + 6n) are unused
typedef union {
    sha3_sheet s[5];
    uint64_t arr[30];
} sha3_state;

static inline void print_sheet(sha3_sheet c)
{
    printf(" < [%lu, %lu], [%lu, %lu], [%lu, %lu] > \n", c.vec.a.vx1[VEC_DW_H],
           c.vec.a.vx1[VEC_DW_L], c.vec.b.vx1[VEC_DW_H], c.vec.b.vx1[VEC_DW_L], c.vec.c.vx1[VEC_DW_H],
           c.vec.c.vx1[VEC_DW_L]);
}


static inline void sha3_sheet_swpl_one(sha3_sheet *s)
{
    const vui128_t vperm_mask = CONST_VINT128(VEC_PERM_0_MASK_L, VEC_PERM_1_MASK_H);
    s->vec.c.vx1[VEC_DW_L] = s->vec.a.vx1[VEC_DW_H];
    s->vec.a.vx1 = vec_perm(s->vec.a.vx1, s->vec.b.vx1, vperm_mask);
    s->vec.b.vx1 = vec_perm(s->vec.b.vx1, s->vec.c.vx1, vperm_mask);
    s->vec.a.vx1[VEC_DW_H]; = s->vec.c.vx1[VEC_DW_L];
}

static inline void sha3_sheet_swpl_two(sha3_sheet *s)
{
    uint64_t tmp = s->vec.a.vx1[VEC_DW_L];
    s->vec.c.vx1[VEC_DW_L] = s->vec.a.vx1[VEC_DW_H];
    s->vec.a.vx1 = s->vec.b.vx1;
    s->vec.b.vx1 = s->vec.c.vx1;
    s->vec.c.vx1[VEC_DW_H] = tmp;
}

static inline void sha3_sheet_swpr_one(sha3_sheet *s)
{
    const vui128_t vperm_mask = CONST_VINT128(VEC_PERM_0_MASK_L, VEC_PERM_1_MASK_H);
    s->vec.c.vx1[VEC_DW_L] = s->vec.b.vx1[VEC_DW_L];
    s->vec.b.vx1 = vec_perm(s->vec.a.vx1, s->vec.b.vx1, vperm_mask);
    s->vec.a.vx1 = vec_perm(s->vec.c.vx1, s->vec.a.vx1, vperm_mask);
    s->vec.a.vx1[VEC_DW_H]; = s->vec.c.vx1[VEC_DW_L];
}

static inline void sha3_sheet_rotl(sha3_sheet *s)
{
    const vui128_t shift_mask = CONST_VINT128(1, 1);
    s->vec.a.vx1 = vec_vrld(s->vec.a.vx1, shift_mask);
    s->vec.b.vx1 = vec_vrld(s->vec.b.vx1, shift_mask);
    s->vec.c.vx1 = vec_vrld(s->vec.c.vx1, shift_mask);
}


#endif
