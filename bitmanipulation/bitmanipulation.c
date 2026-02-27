/*
Basic bit manipulation exercises
*/

#include <stdio.h>
#include <stdbool.h>

// 1. Set a specific bit (make it 1) at a given position
void set_bit(unsigned int *val, int pos);

// 2. Clear a specific bit (make it 0) at a given position
void clear_bit(unsigned int *val, int pos);

// 3. Toggle a specific bit (flip it) at a given position
void toggle_bit(unsigned int *val, int pos);

// 4. Check if a bit at a given position is set
bool is_bit_set(unsigned int val, int pos);

// 5. Count the number of set bits (1s) in an integer (Hamming Weight)
int count_set_bits(unsigned int val);

// --- Implementations ---
void set_bit(unsigned int *val, int pos) {
    
}

void clear_bit(unsigned int *val, int pos) {
    
}

void toggle_bit(unsigned int *val, int pos) {
    
}

bool is_bit_set(unsigned int val, int pos) {
    return (val >> pos) & 1U;
}

int count_set_bits(unsigned int val) {
    return __builtin_popcount(val);
}

// --- Test Harness Logic ---

void run_test(const char* test_name, unsigned int actual, unsigned int expected) {
    if (actual == expected) {
        printf("[PASS] %s: Result 0x%X\n", test_name, actual);
    } else {
        printf("[FAIL] %s: Expected 0x%X, but got 0x%X\n", test_name, expected, actual);
    }
}

int main() {
    printf("Starting Bit Manipulation Tests...\n\n");

    unsigned int data = 0x00; // Binary: 0000 0000

    // Test 1: Set Bit
    set_bit(&data, 3); // Should be 0000 1000 (0x08)
    run_test("Set Bit (pos 3)", data, 0x08);

    // Test 2: Toggle Bit
    toggle_bit(&data, 0); // Should be 0000 1001 (0x09)
    run_test("Toggle Bit (pos 0)", data, 0x09);

    // Test 3: Check Bit
    bool is_set = is_bit_set(data, 3);
    run_test("Is Bit Set (pos 3)", is_set, true);

    // Test 4: Clear Bit
    clear_bit(&data, 3); // Should be 0000 0001 (0x01)
    run_test("Clear Bit (pos 3)", data, 0x01);

    // Test 5: Count Bits
    unsigned int test_val = 0xF; // Binary: 1111 (4 bits set)
    int count = count_set_bits(test_val);
    run_test("Count Set Bits (0xF)", count, 4);

    printf("\nTesting Complete.\n");
    return 0;
}