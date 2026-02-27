#include <stdint.h>

// Mocking the STM32 GPIO structure
struct GPIO_TypeDef {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
};

#define GPIOA_BASE (0x40020000UL)
#define PIN_5      (1u << 5)

// The Template Class
template<uintptr_t PortAddr, uint32_t PinMask>
class GpioPin {
public:
    // Force inlining to ensure no function call overhead
    static __attribute__((always_inline)) inline void on() {
        reinterpret_cast<GPIO_TypeDef*>(PortAddr)->BSRR = PinMask;
    }
};

// Create a specific type for our LED
using LedPin = GpioPin<GPIOA_BASE, PIN_5>;

int main() {
    // This call will disappear and become a single STR instruction
    LedPin::on();
    return 0;
}