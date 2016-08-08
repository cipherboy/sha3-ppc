/*
 * Copyright (C) 2016 Alexander Scheel
 *
 * Licensed under the BSD 2-Clause, MIT, and Apache v2 Public Licenses.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "sha3.h"

int cmpr_sheets(sha3_sheet actual, sha3_sheet expected)
{
    return (actual.vec.a.vx1 != expected.vec.a.vx1 || actual.vec.b.vx1 != expected.vec.b.vx1 || actual.vec.c[VEC_DW_H] != expected.vec.c[VEC_DW_H]);
    return 0;
}

int test_swpl_one() {
    printf("test_swpl_one... ");
    sha3_sheet swpl_one_actual;
    swpl_one_actual.vec.a.vx1 = CONST_VINT128(1, 2);
    swpl_one_actual.vec.b.vx1 = CONST_VINT128(3, 4);
    swpl_one_actual.vec.c.vx1 = CONST_VINT128(5, 0);

    sha3_sheet swpl_one_expected;
    swpl_one_expected.vec.a.vx1 = CONST_VINT128(2, 3);
    swpl_one_expected.vec.b.vx1 = CONST_VINT128(4, 5);
    swpl_one_expected.vec.c.vx1 = CONST_VINT128(1, 0);

    sha3_sheet_swpl_one(&swpl_one_actual);

    if (!cmpr_sheets(swpl_one_actual, swpl_one_expected)) {
        printf("Failed!\n");
        printf("Actual: ");
        print_sheet(swpl_one_actual);
        printf("Expected: ");
        print_sheet(swpl_one_expected);

        return 1;
    }

    printf("pass\n");

    return 0;
}

int test_swpl_two() {
    printf("test_swpl_two... ");
    sha3_sheet swpl_two_actual;
    swpl_two_actual.vec.a.vx1 = CONST_VINT128(1, 2);
    swpl_two_actual.vec.b.vx1 = CONST_VINT128(3, 4);
    swpl_two_actual.vec.c.vx1 = CONST_VINT128(5, 0);

    sha3_sheet swpl_two_expected;
    swpl_two_expected.vec.a.vx1 = CONST_VINT128(3, 4);
    swpl_two_expected.vec.b.vx1 = CONST_VINT128(5, 1);
    swpl_two_expected.vec.c.vx1 = CONST_VINT128(2, 0);

    sha3_sheet_swpl_two(&swpl_two_actual);

    if (!cmpr_sheets(swpl_two_actual, swpl_two_expected)) {
        printf("Failed!\n");
        printf("Actual: ");
        print_sheet(swpl_two_actual);
        printf("Expected: ");
        print_sheet(swpl_two_expected);

        return 1;
    }

    printf("pass\n");

    return 0;
}

int test_swpr_one() {
    printf("test_swpr_one... ");
    sha3_sheet swpr_one_actual;
    swpr_one_actual.vec.a.vx1 = CONST_VINT128(1, 2);
    swpr_one_actual.vec.b.vx1 = CONST_VINT128(3, 4);
    swpr_one_actual.vec.c.vx1 = CONST_VINT128(5, 0);

    sha3_sheet swpr_one_expected;
    swpr_one_expected.vec.a.vx1 = CONST_VINT128(2, 3);
    swpr_one_expected.vec.b.vx1 = CONST_VINT128(4, 5);
    swpr_one_expected.vec.c.vx1 = CONST_VINT128(1, 0);

    sha3_sheet_swpr_one(&swpr_one_actual);

    if (!cmpr_sheets(swpr_one_actual, swpr_one_expected)) {
        printf("Failed!\n");
        printf("Actual: ");
        print_sheet(swpr_one_actual);
        printf("Expected: ");
        print_sheet(swpr_one_expected);

        return 1;
    }

    printf("pass\n");

    return 0;
}

int test_rotl() {
    printf("test_rotl... ");
    sha3_sheet rotl_actual;
    rotl_actual.vec.a.vx1 = CONST_VINT128(1, 2);
    rotl_actual.vec.b.vx1 = CONST_VINT128(3, 4);
    rotl_actual.vec.c.vx1 = CONST_VINT128(5, 0);

    sha3_sheet rotl_expected;
    rotl_expected.vec.a.vx1 = CONST_VINT128(2, 4);
    rotl_expected.vec.b.vx1 = CONST_VINT128(6, 8);
    rotl_expected.vec.c.vx1 = CONST_VINT128(10, 0);

    sha3_sheet_rotl(&rotl_actual);

    if (!cmpr_sheets(rotl_actual, rotl_expected)) {
        printf("Failed!\n");
        printf("Actual: ");
        print_sheet(rotl_actual);
        printf("Expected: ");
        print_sheet(rotl_expected);

        return 1;
    }

    printf("pass\n");

    return 0;
}

int main(int argc, char** argv)
{
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        printf("Little endian.\n");
    #else
        printf("Big endian.\n");
    #endif

    printf("Starting tests...\n");

    return test_swpl_one() + test_swpl_two() + test_swpr_one() + test_rotl();
}
