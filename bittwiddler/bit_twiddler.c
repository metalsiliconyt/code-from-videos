/*
The Bit Twiddler Challenge
Objective: Implement high-performance bitwise operations for 32-bit integers 
without using loops or conditional branches.

You must:
- Swap Endianness: Convert a value between Big-Endian and Little-Endian. 
  This is critical when a Little-Endian CPU (like ARM or x86) talks to 
  a network protocol (usually Big-Endian).
- Hamming Weight: Count the number of set bits (1s) in an integer. 
  This is used in cryptography, error correction, and population count algorithms.
  
Efficiency: Use bitwise shifts, masks, and logical operators. 
Avoid for loops to ensure constant-time O(1) execution.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Reverses the byte order of a 32-bit unsigned integer.
 * Example: 0x12345678 -> 0x78563412
 */
uint32_t swap_endian32(uint32_t val);

/**
 * @brief Calculates the Hamming Weight (number of set bits).
 * Implementation: Use a variable-precision SWAR (SIMD Within A Register) algorithm.
 */
int count_set_bits(uint32_t val);

// --- Implementation Placeholders ---
uint32_t swap_endian32(uint32_t val) {
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8) & 0x0000FF00) |
           ((val << 8) & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

int count_set_bits(uint32_t val) {
    return __builtin_popcount(val);
}

// --- Test Harness ---

static int total_failures = 0;

void run_test(int num, const char* desc, bool passed) {
    if (passed) {
        printf("[PASS] Test %d: %s\n", num, desc);
    } else {
        printf("[FAIL] Test %d: %s\n", num, desc);
        total_failures++;
    }
}

int main() {
    printf("--- Running Bit Twiddler Validation ---\n");

    // Test 1: Endian Swap
    uint32_t original = 0x12345678;
    uint32_t swapped  = swap_endian32(original);
    run_test(1, "32-bit Endian Swap", (swapped == 0x78563412));

    // Test 2: Hamming Weight (Set Bits)
    uint32_t bit_pattern = 0xEA; // 1110 1010 (5 bits set)
    int count = count_set_bits(bit_pattern);
    run_test(2, "Hamming Weight Calculation", (count == 5));

    // Test 3: Edge Case (Zero)
    run_test(3, "Zero Value Check", (count_set_bits(0) == 0 && swap_endian32(0) == 0));

    printf("\n---------------------------------------\n");
    if (total_failures == 0) {
        printf("RESULT: ALL TESTS PASSED ✅\n");
    } else {
        printf("RESULT: %d TEST(S) FAILED ❌\n", total_failures);
    }
    printf("---------------------------------------\n");

    return (total_failures == 0) ? 0 : 1;
}

/*
Key Interviewer Follow-up Questions:
Compiler Intrinsics: Why might you use __builtin_bswap32 or __builtin_popcount 
instead of writing your own logic? (Hint: Most modern CPUs have dedicated hardware 
instructions like POPCNT that do this in a single clock cycle).

The "Branchless" Advantage: Why is avoiding if statements or loops beneficial for 
a CPU's instruction pipeline? (Look up "Branch Misprediction").

Endianness in the Wild: If you are writing a driver for an I2C sensor that sends 
a 16-bit temperature value as High-Byte followed by Low-Byte, which endianness is 
that sensor using?

Sign Extension: What happens if you perform a right shift (>>) on a signed integer 
versus an unsigned integer?
*/