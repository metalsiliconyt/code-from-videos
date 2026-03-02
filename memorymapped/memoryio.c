/**
 * MMIO_Explained.c
 * A guide to how C code talks to Silicon.
 */

#include <stdint.h>

// 1. THE "RAW" WAY (Hardcoded Pointers)
// This is exactly what your diagram shows.
void toggle_pin_raw() {
    // Define the address of the GPIOA Output Data Register
    // volatile: Tells the compiler "Don't optimize this! The hardware changes it."
    volatile uint32_t *GPIOA_ODR = (uint32_t *)0x40020014;

    // The Data Bus carries '1' to the address decoder
    *GPIOA_ODR = 0x00000001; // Pin PA0 goes HIGH
    *GPIOA_ODR = 0x00000000; // Pin PA0 goes LOW
}

// 2. THE "PROFESSIONAL" WAY (Memory-Mapped Structures)
// This is how STM32/ARM headers (like CMSIS) actually work.
// The order of members in the struct MUST match the hardware manual exactly.
typedef struct {
    volatile uint32_t MODER;   // Offset 0x00: Mode register
    volatile uint32_t OTYPER;  // Offset 0x04: Output type register
    volatile uint32_t OSPEEDR; // Offset 0x08: Output speed register
    volatile uint32_t PUPDR;   // Offset 0x0C: Pull-up/pull-down register
    volatile uint32_t IDR;     // Offset 0x10: Input data register
    volatile uint32_t ODR;     // Offset 0x14: Output data register <-- THE TARGET
} GPIO_TypeDef;

// Define the Base Address for the GPIOA Peripheral
#define GPIOA_BASE 0x40020000
#define GPIOA      ((GPIO_TypeDef *) GPIOA_BASE)

void toggle_pin_struct() {
    // Step A: Configure Pin 0 as an "Output" 
    // We write to the MODER register (Offset 0x00)
    GPIOA->MODER |= (1 << 0); 

    // Step B: Set the Pin High
    // The compiler calculates (Base 0x40020000 + Offset 0x14) 
    // and sends the data over the bus.
    GPIOA->ODR = 0x00000001; 
}

int main() {
    // In a real MCU, you MUST enable the clock before this works!
    // RCC->AHB1ENR |= (1 << 0); 
    
    toggle_pin_struct();
    return 0;
}