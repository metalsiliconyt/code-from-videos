#include <stdint.h>

/**
 * 1. CONST: Lives in FLASH (Read-Only)
 * The linker puts this in the .rodata section.
 */
const uint32_t FIRMWARE_VERSION = 0x01020304;

/**
 * 2. VOLATILE: Prevents Optimization
 * Tells the compiler: "This can change outside the program flow."
 * Linker-wise, it stays in RAM (.data), but ensures the CPU
 * actually reads the memory address every single time.
 */
volatile uint32_t status_register;

/**
 * 3. STATIC (Global): Restricts Scope
 * Doesn't change memory placement (.data or .bss), but 
 * removes the symbol from the global symbol table. 
 * Other files can't "see" it during linking.
 */
static uint32_t local_state_machine;

void demonstrate_stack_const(void) {
    /**
     * 4. LOCAL CONST: Lives on the STACK (RAM)
     * Even though it's "const", it's a local variable. 
     * The compiler usually treats this as an immediate value 
     * or pushes it onto the Stack at runtime.
     * It will NOT have a permanent entry in the .rodata section.
     */
    const uint32_t local_const = 0xDEADBEEF;

    /**
     * 5. POINTER TO CONST vs CONST POINTER
     * This is a classic interview question.
     */
    const uint32_t *ptr_to_const = &FIRMWARE_VERSION; // Pointer lives in RAM, points to Flash
    uint32_t * const const_ptr = (uint32_t *)&status_register; // Pointer lives in Flash (maybe), points to RAM
}

void main(void) {
    // 6. STATIC (Local): Persistent RAM
    // Unlike a normal local variable (Stack), this lives in 
    // the .data/.bss section of RAM but is only visible in this function.
    static int32_t boot_counter = 0;
    demonstrate_stack_const();

    //boot_counter++;
    status_register = 0x01;
}