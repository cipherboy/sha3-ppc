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

// Vector slot [5 + 6n] are unused
typedef union {
    sha3_sheet s[5];
    uint64_t arr[30];
} sha3_state;

static inline void print_sheet(sha3_sheet c)
{
    printf(" < [%lu, %lu], [%lu, %lu], [%lu, %lu] > \n", c.vec.a.vx[VEC_DW_H],
           c.vec.a.vx[VEC_DW_L], c.vec.b.vx[VEC_DW_H], c.vec.b.vx[VEC_DW_L], c.vec.c.vx[VEC_DW_H],
           c.vec.c.vx[VEC_DW_L]);
}


static inline void sha3_sheet_swpl_one(sha3_sheet *s)
{
    const vui128_t vperm_mask = CONST_VINT128(VPERM_0_MASK_L, VPERM_1_MASK_H);
    s->vec.c.vx[VEC_DW_L] = s->vec.a.vx[VEC_DW_H];
    s->vec.a.vx = vec_perm(s->vec.a.vx, s->vec.b.vx, vperm_mask);
    s->vec.b.vx = vec_perm(s->vec.b.vx, s->vec.c.vx, vperm_mask);
    s->vec.a.vx[VEC_DW_H]; = s->vec.c.vx[VEC_DW_L];
}

static inline void sha3_sheet_swpl_two(sha3_sheet *s)
{
    uint64_t tmp = s->vec.a.vx[VEC_DW_L];
    s->vec.c.vx[VEC_DW_L] = s->vec.a.vx[VEC_DW_H];
    s->vec.a.vx = s->vec.b.vx;
    s->vec.b.vx = s->vec.c.vx;
    s->vec.c.vx[VEC_DW_H] = tmp;
}

static inline void sha3_sheet_swpr_one(sha3_sheet *s)
{
    const vui128_t vperm_mask = CONST_VINT128(VPERM_0_MASK_L, VPERM_1_MASK_H);
    s->vec.c.vx[VEC_DW_L] = s->vec.b.vx[VEC_DW_L];
    s->vec.b.vx = vec_perm(s->vec.a.vx, s->vec.b.vx, vperm_mask);
    s->vec.a.vx = vec_perm(s->vec.c.vx, s->vec.a.vx, vperm_mask);
    s->vec.a.vx[VEC_DW_H]; = s->vec.c.vx[VEC_DW_L];
}

static inline void sha3_sheet_rotl(sha3_sheet *s)
{
    const vui128_t shift_mask = CONST_VINT128(1, 1);
    s->vec.a.vx = vec_vrld(s->vec.a.vx, shift_mask);
    s->vec.b.vx = vec_vrld(s->vec.b.vx, shift_mask);
    s->vec.c.vx = vec_vrld(s->vec.c.vx, shift_mask);
}

static inline void sha3_sheet_rotl_var(sha3_sheet *s, int shift_high, int shift_low)
{
    const vui128_t shift_mask;

    s->vec.a.vx = vec_vrld(s->vec.a.vx, shift_mask);
    s->vec.b.vx = vec_vrld(s->vec.b.vx, shift_mask);
    s->vec.c.vx = vec_vrld(s->vec.c.vx, shift_mask);
}

static inline void sha3_theta(sha3_state *A)
{
    sha3_sheet c;
    sha3_sheet c_c;
    sha3_sheet d;
    int i = 0;

    // c = s[0] ^ s[1] ^ s[2] ^ s[3] ^ s[4]
    c.vec.a.vx = vec_xor(A->s[0].vec.a.vx, A->s[1].vec.a.vx);
    c.vec.b.vx = vec_xor(A->s[0].vec.b.vx, A->s[1].vec.b.vx);
    c.vec.c.vx = vec_xor(A->s[0].vec.c.vx, A->s[1].vec.c.vx);

    c.vec.a.vx = vec_xor(c.vec.a.vx, A->s[2].vec.a.vx);
    c.vec.b.vx = vec_xor(c.vec.b.vx, A->s[2].vec.b.vx);
    c.vec.c.vx = vec_xor(c.vec.c.vx, A->s[2].vec.c.vx);

    c.vec.a.vx = vec_xor(c.vec.a.vx, A->s[3].vec.a.vx);
    c.vec.b.vx = vec_xor(c.vec.b.vx, A->s[3].vec.b.vx);
    c.vec.c.vx = vec_xor(c.vec.c.vx, A->s[3].vec.c.vx);

    c.vec.a.vx = vec_xor(c.vec.a.vx, A->s[4].vec.a.vx);
    c.vec.b.vx = vec_xor(c.vec.b.vx, A->s[4].vec.b.vx);
    c.vec.c.vx = vec_xor(c.vec.c.vx, A->s[4].vec.c.vx);

    // copy c into c_c
    c.vec.c.ord[VEC_DW_H] = 0;
    c_c.vec.a.vx1 = c.vec.a.vx1;
    c_c.vec.b.vx1 = c.vec.b.vx1;
    c_c.vec.c.vx1 = c.vec.c.vx1;

    // c_c = <ROTL(c[1], 1), ROTL(c[2], 1), ROTL(c[3], 1), ROTL(c[4], 1), ROTL(c[0], 1)>
    // c = <c[4], c[0], c[1], c[2], c[3]>

    sha3_sheet_swpr_one(&c);
    sha3_sheet_swpl_one(&c_c);
    sha3_sheet_rotl(c_c);

    // d = c1 ^ c2
    d.vec.a.vx = vec_xor(c_c.vec.a.vx, c.vec.a.vx);
    d.vec.b.vx = vec_xor(c_c.vec.b.vx, c.vec.b.vx);
    d.vec.c.vx = vec_xor(c_c.vec.c.vx, c.vec.c.vx);

    // xor d back into state
    for (; i < 5; i++) {
        A->s[i].vec.a.vx = vec_xor(A->s[i].vec.a.vx, d.vec.a.vx);
        A->s[i].vec.b.vx = vec_xor(A->s[i].vec.b.vx, d.vec.b.vx);
        A->s[i].vec.c.vx = vec_xor(A->s[i].vec.c.vx, d.vec.c.vx);
    }
}

static inline void sha3_chi(sha3_state *A) {
    sha3_sheet a;
    sha3_sheet b;
    int i = 0

    for (; i < 5; i++) {
        // duplicate s[i] into a, b
        a.vec.a.vx1 = A->s[i].vec.a.vx1;
        b.vec.a.vx1 = A->s[i].vec.a.vx1;
        a.vec.b.vx1 = A->s[i].vec.b.vx1;
        b.vec.b.vx1 = A->s[i].vec.b.vx1;
        a.vec.c.vx1 = A->s[i].vec.c.vx1;
        b.vec.c.vx1 = A->s[i].vec.c.vx1;

        // a = <a[1], a[2], a[3], a[4], a[0]>
        // b = <b[2], b[3], b[4], b[0], b[1]>
        sha3_sheet_swpl_one(&a);
        sha3_sheet_swpl_two(&b);

        // a = b & ~a
        a.vec.a.vx1 = vec_andc(b.vec.a.vx1, a.vec.a.vx1);
        a.vec.b.vx1 = vec_andc(b.vec.b.vx1, a.vec.b.vx1);
        a.vec.c.vx1 = vec_andc(b.vec.c.vx1, a.vec.c.vx1);

        A->s[i].vec.a.vx = vec_xor(A->s[i].vec.a.vx, a.vec.a.vx);
        A->s[i].vec.b.vx = vec_xor(A->s[i].vec.b.vx, a.vec.b.vx);
        A->s[i].vec.c.vx = vec_xor(A->s[i].vec.c.vx, a.vec.c.vx);
    }
}

static inline void sha3_rho(sha3_state *A) {
    sha3_sheet_rotl_var(&A->s[0].vec.a.vx, 0, 1);
    sha3_sheet_rotl_var(&A->s[0].vec.b.vx, 62, 28);
    sha3_sheet_rotl_var(&A->s[0].vec.c.vx, 27, 0);

    sha3_sheet_rotl_var(&A->s[1].vec.a.vx, 36, 44);
    sha3_sheet_rotl_var(&A->s[1].vec.b.vx, 6, 55);
    sha3_sheet_rotl_var(&A->s[1].vec.c.vx, 20, 0);

    sha3_sheet_rotl_var(&A->s[2].vec.a.vx, 3, 10);
    sha3_sheet_rotl_var(&A->s[2].vec.b.vx, 43, 25);
    sha3_sheet_rotl_var(&A->s[2].vec.c.vx, 39, 0);

    sha3_sheet_rotl_var(&A->s[3].vec.a.vx, 41, 45);
    sha3_sheet_rotl_var(&A->s[3].vec.b.vx, 15, 21);
    sha3_sheet_rotl_var(&A->s[3].vec.c.vx, 8, 0);

    sha3_sheet_rotl_var(&A->s[4].vec.a.vx, 18, 2);
    sha3_sheet_rotl_var(&A->s[4].vec.b.vx, 61, 56);
    sha3_sheet_rotl_var(&A->s[4].vec.c.vx, 14, 0);
}

#endif
