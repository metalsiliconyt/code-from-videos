/***
 * Firmware Technical Interview Question
 * 
 * Bit Streaming:
 * 
 * In embedded systems, bandwidth is often limited. 
 * You are tasked with writing a utility function 
 * that processes a stream of 8-bit sensor readings.
 * 
 * Extract the 5 least significant bits (LSBs) of each input byte 
 * and pack them contiguously into the output buffer.
 * You must pack these 5-bit chunks back-to-back so there are no gaps in the output array. 
 *   
 * Example:
 *  Input (2 bytes):
 *  - [[Byte 0], [Byte 1]]
 *  - Byte 0: [A B C D E F G H] (10101110)
 *  - Byte 1: [I J K L M N O P] (00001101)
 *  Output:
 *  - [[D E F G H L M N] [O P 0 0 0 0 0 0]] ([[01110011] [01000000]])
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

/**
 * @param input     Pointer to the source 8-bit sensor data
 * @param input_len Number of elements in the input array
 * * @return         >>> What should we do here??? <<<
 */
void pack_sensor_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_len);

void pack_sensor_data(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_len)
{
    uint8_t current_byte = 0;
    int8_t current_bit = 7;

    for (int i = 0; i < input_len; i++)
    {
        uint8_t post_masked_data = input[i] & 0x1F;

        for (int j = 4; j >= 0; j--)
        {
            // Select current bit to copy over
            uint8_t bit_to_out = (post_masked_data >> j) & 0x01;

            // Copy bit_to_out to our output buffer
            output[current_byte] |= bit_to_out << current_bit;

            current_bit--;

            if (current_bit < 0)
            {
                current_byte++;
                current_bit = 7;
            }
        }
    }
}


/**
 * Key Interviewer Follow-up Questions
 * - How do we handle memory allocation for output data?
 * - How do you handle the very last byte if the total number of bits isn't a multiple of 8?
 * - What is the Big-O complexity of your solution?
 * - How would your code change if the shift amount was dynamic instead of a constant 3?
 */

// --- Test Utilities ---
void print_hex(const char* label, const uint8_t* buf, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("0x%02X ", buf[i]);
    }
    printf("\n");
}

int main() {
    printf("--- Running Firmware Interview Tests ---\n\n");

    // Case 1: The Example from the Prompt
    // Byte 0: 10101110 (0xAE) -> 5 bits: 01110 (0x0E)
    // Byte 1: 00001101 (0x0D) -> 5 bits: 01101 (0x0D)
    // Expected: [01110 011] [01 000000] -> [0x73, 0x40]
    uint8_t input1[] = {0xAE, 0x0D};
    uint8_t output1[2] = {0};
    uint8_t expected1[] = {0x73, 0x40};
    
    pack_sensor_data(input1, 2, output1, 2);
    
    print_hex("Test 1 Input", input1, 2);
    print_hex("Test 1 Result", output1, 2);
    assert(2 == 2);
    assert(memcmp(output1, expected1, 2) == 0);
    printf("Test 1 Passed!\n\n");

    // Case 2: Single Byte (Partial Output)
    // 0xFF -> 5 bits are 11111 (0x1F)
    // Expected: [11111 000] -> [0xF8]
    uint8_t input2[] = {0xFF};
    uint8_t output2[1] = {0};
    uint8_t expected2[] = {0xF8};

    pack_sensor_data(input2, 1, output2, 1);
    
    print_hex("Test 2 Input", input2, 1);
    print_hex("Test 2 Result", output2, 1);
    assert(1 == 1);
    assert(output2[0] == expected2[0]);
    printf("Test 2 Passed!\n\n");

    // Case 3: Eight 5-bit chunks (Should fill exactly 5 bytes)
    // 8 * 5 = 40 bits = 5 bytes.
    uint8_t input3[8];
    memset(input3, 0x1F, 8); // All 5-bit chunks are 11111
    uint8_t output3[5] = {0};
    
    pack_sensor_data(input3, 8, output3, 5);
    assert(5 == 5);
    for(int i=0; i<5; i++) assert(output3[i] == 0xFF);
    printf("Test 3 Passed (Perfect alignment)!\n\n");

    printf("ALL TESTS PASSED SUCCESSFULLY\n");
    return 0;
}