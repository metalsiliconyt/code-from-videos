//
// Temp monitor device with added user authentication
// (States required)
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

typedef enum {
    STATE_LOCKED,      // Device is waiting for a PIN
    STATE_IDLE,        // Device is unlocked, waiting for timer
    STATE_SENSING,     // Currently reading/processing data
    STATE_ERROR        // Something went wrong (e.g., sensor failure)
} DeviceState_t;

// Variable to track current state
DeviceState_t current_state = STATE_LOCKED;

void run_device_fsm(void) {
    switch (current_state) {
        
        case STATE_LOCKED:
            if (is_correct_pin_entered()) {
                update_display_text("Unlocked");
                current_state = STATE_IDLE; // Transition
            }
            break;

        case STATE_IDLE:
            if (timer_triggered) {
                timer_triggered = false;
                current_state = STATE_SENSING; // Transition
            }
            if (button_pressed) {
                button_pressed = false;
                current_state = STATE_LOCKED; // Manual Relock
            }
            break;

        case STATE_SENSING:
            if (read_sensor_success()) {
                update_display_data();
                current_state = STATE_IDLE; // Return to IDLE
            } else {
                current_state = STATE_ERROR; // Handle Failure
            }
            break;

        case STATE_ERROR:
            handle_system_reset();
            current_state = STATE_LOCKED;
            break;
    }
}

int main(void) {
    hardware_init();

    while (1) {
        // The FSM manages the "Brain" of the device
        run_device_fsm();

        // Optional: Low-power mode (Sleep until next interrupt)
        enter_low_power_mode(); 
    }
}