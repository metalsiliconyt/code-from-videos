#include <stdio.h>
#include <stdint.h>
#include <string.h>

/**
 * EXAMPLE 1: Data Grouping (The "Too Many Arguments" Problem)
 * * Without a struct, passing related data requires long, error-prone 
 * function signatures. Structs allow you to pass a single pointer.
 */

struct UserProfile {
    char username[32];
    uint32_t id;
    int16_t x_pos;
    int16_t y_pos;
    uint8_t level;
};

// BAD: Hard to read and maintain
void update_position_old(int16_t x, int16_t y, uint32_t id, uint8_t level) { /* ... */ }

// GOOD: Clean, extensible, and passes a single memory address
void update_profile(struct UserProfile *p) {
    p->x_pos += 10;
    p->y_pos -= 5;
    printf("Updated %s to (%d, %d)\n", p->username, p->x_pos, p->y_pos);
}


/**
 * EXAMPLE 2: Memory Mapping (Hardware Abstraction)
 * * In embedded systems, hardware registers are at specific memory addresses.
 * Structs allow you to overlay a template directly onto that memory.
 */

struct TimerRegisters {
    uint32_t CONTROL; // Offset 0x00
    uint32_t RELOAD;  // Offset 0x04
    uint32_t VALUE;   // Offset 0x08
    uint32_t INTERRUPT; // Offset 0x0C
};

void init_timer() {
    // Hypothetical hardware address
    volatile struct TimerRegisters *timer = (struct TimerRegisters *)0x40001000;
    
    timer->RELOAD = 0xFFFF; // Direct hardware access via struct members
    timer->CONTROL |= 0x01; 
}


/**
 * EXAMPLE 3: Memory Alignment and Padding
 * * Structs give you control over how data sits in RAM, which is 
 * vital for performance and protocol buffers.
 * Why this is critical for Low-Level work:
 * Binary Compatibility: If you are sending a struct over a network (like a TCP packet), the computer on the other end must have the exact same padding, or the data will be shifted and unreadable.
 * Memory Constraints: In embedded systems (like a microwave controller), RAM is tiny. If you order your struct poorly, you might waste 25% of your memory just on padding.
 * The __attribute__((packed)) Trick: In low-level C, you can tell the compiler "Do not pad this!" using a special attribute. This is common when matching hardware protocols where every bit counts.
 * Pro-Tip: To minimize padding naturally, always order your struct members from largest to smallest (e.g., uint64_t, then uint32_t, then char). This usually lets the smaller variables "fill the gaps" left by the larger ones.
 */

struct OptimizedData {
    uint32_t large; // 4 bytes
    uint16_t med;   // 2 bytes
    uint8_t small;  // 1 byte
    // Compiler adds 1 byte of padding here to keep 4-byte alignment
};

/**
 * EXAMPLE 4: Linked Lists (Dynamic Data Structures)
 * * Low-level programming often requires custom containers. 
 * Self-referential structs make this possible.
 */

struct Node {
    int data;
    struct Node *next; // The struct "points" to another of its own kind
};


/**
 * EXAMPLE 5: Type Punning with Unions/Structs
 * * Often used in networking or graphics to treat the same bits 
 * of data in different ways (e.g., a color as one 32-bit int or four 8-bit channels).
 */

struct Color {
    uint8_t r, g, b, a;
};

union Pixel {
    uint32_t raw;
    struct Color channels;
};

void process_pixel() {
    union Pixel p;
    p.raw = 0xFF00AAFF; // Set all at once
    p.channels.r = 0xBB; // Modify just the Red channel
}

int main() {
    // Example 1 Demo
    struct UserProfile user1 = {"PlayerOne", 101, 0, 0, 1};
    update_profile(&user1);

    printf("Struct Importance Demo Complete.\n");
    return 0;
}