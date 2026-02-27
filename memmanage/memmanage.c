/*
  C Memory Model Proficiency Test
  Objective: Identify and correctly utilize different memory segments.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Segment Identification Part ---

// TODO: The candidate should identify which segment these variables live in.
int global_uninitialized;             // 1. Where does this live? (Static, .bss)
int global_initialized = 42;          // 2. Where does this live? (Static, .data)
static int static_var = 100;          // 3. Where does this live? (Static, .data)
const char* text_ptr = "Hello World"; // 4. Where does the STRING LITERAL live? (Static, .rodata)

// --- Functional Exercises ---

/**
 * Exercise A: The Stack & Pointer Safety
 * This function is intended to initialize a local variable and return its address.
 * TODO: Fix the bug and explain why the original approach fails in the memory model.
 */
int* create_integer_on_stack(int value) {
    int* out_value = malloc(sizeof(int));
    *out_value = value;
    return out_value;
}

/**
 * Exercise B: The Heap
 * TODO: Allocate memory for an array of 'n' integers, initialize them to zero, 
 * and return the pointer. Ensure the candidate handles potential allocation failure.
 */
int* create_array_on_heap(int n) {
    int* array = calloc(n, sizeof(int));
    return array;
}

/**
 * Exercise C: .bss vs .data Persistence
 * Increment a counter that persists across function calls
 * without using a global variable.
 */
int increment_counter() {
    static int counter = 0;
    counter++;
    return counter;
}

// --- Test Harness ---

void run_test(const char* test_name, int condition) {
    if (condition) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
    }
}

int main() {
    printf("Starting C Memory Model Tests...\n\n");

    // Test 1: Stack Persistence (The candidate must solve Exercise A properly)
    // Note: A correct implementation would likely use heap or static memory.
    int* p_stack = create_integer_on_stack(10);
    // run_test("Stack address validity", p_stack != NULL); 

    // Test 2: Heap Allocation
    int* heap_array = create_array_on_heap(5);
    if (heap_array != NULL) {
        run_test("Heap Allocation", heap_array[0] == 0);
        free(heap_array); // Memory management check
    } else {
        run_test("Heap Allocation", 0);
    }

    // Test 3: Static/Persistence Check
    increment_counter();
    increment_counter();
    int count = increment_counter();
    run_test("Static Variable Persistence", count == 3);

    // Test 4: Segment Knowledge Question (Interview Check)
    printf("\n--- Oral/Written Follow-up ---\n");
    printf("1. If I change 'text_ptr[0] = 'h'', why does the program crash?\n");
    printf("2. What is the difference between malloc() and calloc() regarding the .bss segment logic?\n");

    printf("\nTesting Complete.\n");
    return 0;
}