//
// Temp monitor device with a display
// 

#include <stdint.h>
#include <stdbool.h>

// Global flags (volatile because they change in ISR)
volatile bool timer_triggered = false;
volatile bool button_pressed = false;

// Mock function prototypes
void hardware_init(void);
void read_sensor(void);
void update_display(void);
void process_button_event(void);
void go_to_sleep(void);

// This runs every 100ms via a hardware timer interrupt
void Timer_ISR(void) {
    timer_triggered = true; 
}

// This runs when a physical button is pressed
void GPIO_Button_ISR(void) {
    button_pressed = true;
}

int main(void) {
    hardware_init();

    while (1) {
        // 1. Task: Periodic Sensor Reading
        if (timer_triggered) {
            timer_triggered = false; // Reset flag
            read_sensor();
            update_display();
        }

        // 2. Task: Asynchronous User Input
        if (button_pressed) {
            button_pressed = false; // Reset flag
            process_button_event();
        }

        // 3. Optional: Power Management
        // If no flags are set, the CPU can "Wait For Interrupt" (WFI)
        if (!timer_triggered && !button_pressed) {
            go_to_sleep(); 
        }
    }

    return 0; // Never reached
}