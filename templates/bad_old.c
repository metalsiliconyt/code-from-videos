#include <stdint.h>

// Mocking the STM32 GPIO structure
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR; // Bit Set/Reset Register
} GPIO_TypeDef;

#define GPIOA_BASE (0x40020000UL)
#define GPIOA      ((GPIO_TypeDef *) GPIOA_BASE)
#define PIN_5      (1u << 5)

// The C Function
// The compiler must assume 'port' and 'pin' could change at runtime
void inline pin_on_c(GPIO_TypeDef* port, uint32_t pin) {
    port->BSRR = pin;
}

int main() {
    pin_on_c(GPIOA, PIN_5);
    return 0;
}