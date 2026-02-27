// test.c
#include <stdio.h>
#include <stdint.h>
#include <stdatomic.h>
#include <string.h>

/* ============================================================
 * volatile
 * Force a load from memory instead of cached value
 * ============================================================ */

volatile uint32_t volatile_reg = 0;

int test_volatile(void) {
    volatile_reg = 1;
    uint32_t a = volatile_reg;
    volatile_reg = 2;
    uint32_t b = volatile_reg;

    return (a == 1 && b == 2);
}

/* ============================================================
 * const
 * Tells compiler this value is read-only
 * ============================================================ */

int test_const(void) {
    const int x = 42;
    return (x == 42);
}

/* ============================================================
 * restrict (C only)
 * Pointer is the only reference to this memory
 * ============================================================ */

void restrict_copy(int * restrict dst,
                   const int * restrict src,
                   size_t n)
{
    for (size_t i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

int test_restrict(void) {
    int src[3] = {1, 2, 3};
    int dst[3] = {0};

    restrict_copy(dst, src, 3);

    return memcmp(src, dst, sizeof(src)) == 0;
}

/* ============================================================
 * _Atomic / atomic_*
 * Ensures atomic modification and ordering
 * ============================================================ */

atomic_int atomic_counter;

int test_atomic(void) {
    atomic_store(&atomic_counter, 0);

    atomic_fetch_add(&atomic_counter, 1);
    atomic_fetch_add(&atomic_counter, 1);

    return atomic_load(&atomic_counter) == 2;
}

/* ============================================================
 * inline
 * Suggests compiler to inline / remove call overhead
 * ============================================================ */

static inline int add_inline(int a, int b) {
    return a + b;
}

int test_inline(void) {
    int x = add_inline(2, 3);
    return x == 5;
}

/* ============================================================
 * static
 * - Limits symbol visibility (functions)
 * - Static storage duration (variables)
 * ============================================================ */


int increment_static(void) {
    static int static_counter = 0;
    static_counter++;
    return static_counter;
}

int test_static(void) {
    int output = 0;
    increment_static();
    output = increment_static();

    return output == 2;
}

/* ============================================================
 * extern
 * Variable defined elsewhere (simulated here)
 * ============================================================ */

extern int extern_value;
int extern_value = 99;

int test_extern(void) {
    return extern_value == 99;
}

/* ============================================================
 * Test Harness
 * ============================================================ */

#define RUN_TEST(fn) \
    printf("%-20s : %s\n", #fn, fn() ? "PASS" : "FAIL")

int main(void) {
    printf("C Keyword Knowledge Tests\n");
    printf("=========================\n");

    RUN_TEST(test_volatile);
    RUN_TEST(test_const);
    RUN_TEST(test_restrict);
    RUN_TEST(test_atomic);
    RUN_TEST(test_inline);
    RUN_TEST(test_static);
    RUN_TEST(test_extern);

    return 0;
}
