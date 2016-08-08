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

typedef union {
    sha3_sheet s[5];
    uint64_t arr[30];
} sha3_state;

static inline void print_sheet(sha3_sheet c)
{
    printf(" < [%lu, %lu], [%lu, %lu], [%lu, %lu] > \n", c.vec.a[VEC_DW_H],
           c.vec.a[VEC_DW_L], c.vec.b[VEC_DW_H], c.vec.b[VEC_DW_L], c.vec.c[VEC_DW_H],
           c.vec.c[VEC_DW_L]);
}


static inline void sha3_sheet_rotl_one(sha3_sheet *s)
{
    const p_v128 vperm_mask = CONST_VINT128(VPERM_0_MASK_L, VPERM_1_MASK_H);
    s->vec.c[VEC_DW_L] = s->vec.a[VEC_DW_H];
    vec_perm()
}

static inline void sha3_sheet_rotl_two(sha3_sheet *s)
{
    uint64_t tmp = s->vec.a[VEC_DW_L];
    s->vec.c[VEC_DW_L] = s->vec.a[VEC_DW_H];
    s->vec.a.vx1 = s->vec.b.vx1;
    s->vec.b.vx1 = s->vec.c.vx1;
    s->vec.c[VEC_DW_H] = tmp;
}


#endif
