/*
The "Ouroboros" Ring Buffer Challenge
Objective: Implement a robust circular buffer for a resource-constrained embedded system.
You must:
1. Initialize a buffer with a fixed capacity.
2. Implement 'Push' (enqueue) logic: Add data to the tail. If the buffer is full, 
   the operation should fail (or overwrite, depending on policy—here, we will return an error).
3. Implement 'Pop' (dequeue) logic: Remove data from the head.
4. Ensure the state (head, tail, count) is correctly updated after every operation.

Note: In a real firmware environment, this would often be used to bridge an ISR 
and a main loop.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    uint8_t *buffer;    // Pointer to the data array
    volatile size_t head;        // Index of the oldest element (to pop)
    volatile size_t tail;        // Index of the next available slot (to push)
    size_t capacity;    // Maximum capacity
} circ_buf_t;

typedef enum {
    CB_OK = 0,
    CB_EMPTY,
    CB_FULL,
    CB_INVALID
} cb_status_t;

/**
 * @param cb        Pointer to the handle
 * @param raw_mem   Pre-allocated memory block
 * @param cap       Size of raw_mem
 */
void circ_buf_init(circ_buf_t *cb, uint8_t *raw_mem, size_t cap);

/**
 * @brief Adds an element to the buffer
 * @return CB_OK on success, CB_FULL if no space
 */
cb_status_t circ_buf_push(circ_buf_t *cb, uint8_t data);

/**
 * @brief Removes an element from the buffer
 * @param data      Pointer to store the retrieved byte
 * @return CB_OK on success, CB_EMPTY if no data
 */
cb_status_t circ_buf_pop(circ_buf_t *cb, uint8_t *data);

// --- Implementation Placeholders ---

void circ_buf_init(circ_buf_t *cb, uint8_t *raw_mem, size_t cap) {
    if (cb == NULL) return;
    cb->buffer = raw_mem;
    cb->capacity = cap;
    cb->head = 0;
    cb->tail = 0;
}

cb_status_t circ_buf_push(circ_buf_t *cb, uint8_t data) {
    
    size_t next = (cb->tail + 1) % cb->capacity;

    if (next == cb->head)
    {
        return CB_FULL;
    }
    // Add data at tail and wrap tail index
    cb->buffer[cb->tail] = data;
    cb->tail = next;
    return CB_OK; 
}

cb_status_t circ_buf_pop(circ_buf_t *cb, uint8_t *data) {
    // Check if empty
    if (cb->tail == cb->head)
    {
        return CB_EMPTY;
    }

    // Retrieve data from head and wrap head index
    if (data != NULL)
    {
        *data = cb->buffer[cb->head];
    }

    cb->head = (cb->head + 1) % cb->capacity;
    return CB_OK;
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
    printf("--- Running Circular Buffer Validation ---\n");

    uint8_t memory[4];
    circ_buf_t cb;
    circ_buf_init(&cb, memory, 4);

    // Test 1: Basic Push/Pop
    circ_buf_push(&cb, 0xA);
    uint8_t val;
    cb_status_t s1 = circ_buf_pop(&cb, &val);
    run_test(1, "Basic Push/Pop", (s1 == CB_OK && val == 0xA));

    // Test 2: Full Buffer Condition
    circ_buf_push(&cb, 1);
    circ_buf_push(&cb, 2);
    circ_buf_push(&cb, 3);
    circ_buf_push(&cb, 4);
    cb_status_t s2 = circ_buf_push(&cb, 5); // Should be full
    run_test(2, "Buffer Full Detection", (s2 == CB_FULL));

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

- Thread Safety: If a producer (main loop) and a consumer (Interrupt) 
  access this buffer, which variables need to be 'volatile'? 
  Would you need a critical section for 'count'?
- Power-of-Two Optimization: If the capacity is always a power of two 
  (e.g., 16, 64, 256), how can you replace the modulo operator (%) 
  or 'if' checks for wrapping with a bitwise AND?
- Zero-Copy: How would you modify the Pop function to return a pointer 
  to the data instead of copying the data out? What are the risks?
- Empty vs Full: Many implementations avoid a 'count' variable and instead 
  leave one slot empty to distinguish between Full and Empty. 
  What are the pros and cons of this approach?


Closing Comments:

- If an interviewer asks "What is the best way?", the answer is usually "It depends on the concurrency model."
- SPSC (Single Producer Single Consumer): If one thread only writes and the other only reads, use the One-Slot Gap method. 
  It allows for "lock-free" operation, which is much faster and avoids disabling interrupts.
- Multi-Producer: If multiple tasks are trying to push data (e.g., multiple sensors), 
  you must use a mutex or a critical section to prevent two tasks from grabbing the same tail index.
- Overwrite Policy: Ask the interviewer: "Should I drop the new data when full, or overwrite the oldest data?" 
  Overwriting is common in logging (keep the most recent data), while dropping is common in command processing (ensure every command is processed).
*/