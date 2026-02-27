/*
Implement Button Debouncing
Objective: Implement a non-blocking debouncer for a mechanical push button.

The Reality: Mechanical switches don't provide a clean signal. When pressed, 
the metal contacts "bounce" against each other for several milliseconds, 
creating a flurry of false high/low transitions.

Constraints:
- No Delay: You cannot use sleep() or delay_ms().
- Non-Blocking: The check_button function must return immediately.
- State Machine: Track the button state (IDLE, DEBOUNCING, PRESSED).

Logic Flow:
1. Detect a change in GPIO state.
2. Start a timer (record the current timestamp).
3. If the state remains stable for X milliseconds, confirm the press.
4. If the state changes again before the timer expires, reset the timer.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Simulated System Time (incremented in test harness)
uint32_t system_millis = 0;

// Configuration
#define DEBOUNCE_THRESHOLD_MS 50
#define BUTTON_PIN_LOW  0
#define BUTTON_PIN_HIGH 1

typedef enum {
    STATE_RELEASED,
    STATE_MAYBE_PRESSED,
    STATE_PRESSED,
    STATE_MAYBE_RELEASED
} button_state_t;

typedef struct {
    button_state_t state;
    uint32_t last_tick;
    bool stable_output;
} debouncer_t;

/**
 * @brief Processes the raw GPIO input and updates the debouncer state.
 * @param raw_level The current physical level of the GPIO (0 or 1).
 * @return true if the button is considered "stable pressed".
 */
bool update_debounce(debouncer_t *db, int raw_level);

// --- Implementation Placeholder ---

bool update_debounce(debouncer_t *db, int raw_level) {
    // TODO: Implement a Finite State Machine (FSM) here
    // Use system_millis to track elapsed time

    bool is_active = (raw_level == BUTTON_PIN_HIGH);

    switch (db->state) {
        case STATE_RELEASED:
            if (is_active) {
                db->state = STATE_MAYBE_PRESSED;
                db->last_tick = system_millis;
            }
            break;
        
        case STATE_MAYBE_PRESSED:
            if (!is_active) {
                db->state = STATE_RELEASED;
            } else if ((system_millis - db->last_tick) >= DEBOUNCE_THRESHOLD_MS) {
                db->state = STATE_PRESSED;
                return true;
            }
            break;

        case STATE_PRESSED:
            if (!is_active) {
                db->state = STATE_MAYBE_RELEASED;
                db->last_tick = system_millis;
            }
            break;

        case STATE_MAYBE_RELEASED:
            if (is_active) {
                db->state = STATE_PRESSED;
            } else if ((system_millis - db->last_tick) >= DEBOUNCE_THRESHOLD_MS) {
                db->state = STATE_RELEASED;
            }
            break;
    }

    return (db->state == STATE_PRESSED);
}

// --- Test Harness ---

static int total_failures = 0;

void verify(const char* desc, bool condition) {
    if (condition) {
        printf("[PASS] %s\n", desc);
    } else {
        printf("[FAIL] %s\n", desc);
        total_failures++;
    }
}

int main() {
    printf("--- Running Debouncer Validation ---\n");

    debouncer_t my_button = { STATE_RELEASED, 0, false };

    // Scenario 1: Quick Noise (Should NOT trigger)
    system_millis = 10;
    update_debounce(&my_button, BUTTON_PIN_HIGH); // Initial noise
    system_millis = 20;
    bool noise_result = update_debounce(&my_button, BUTTON_PIN_LOW); // Drops back down
    verify("Ignore noise shorter than threshold", noise_result == false);

    // Scenario 2: Valid Press (Stable high for > threshold)
    system_millis = 100;
    update_debounce(&my_button, BUTTON_PIN_HIGH); // Start press
    system_millis = 160; // 60ms have passed (Threshold is 50)
    bool press_result = update_debounce(&my_button, BUTTON_PIN_HIGH);
    verify("Confirm stable press after threshold", press_result == true);

    // Scenario 3: Contact Bounce during Release
    system_millis = 200;
    update_debounce(&my_button, BUTTON_PIN_LOW); // Starts releasing
    system_millis = 210;
    update_debounce(&my_button, BUTTON_PIN_HIGH); // Bounces back high
    system_millis = 270;
    bool release_result = update_debounce(&my_button, BUTTON_PIN_LOW);
    verify("Stay 'pressed' if release bounces", my_button.state != STATE_RELEASED);

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
1. Interrupts vs. Polling: If you used an Interrupt (ISR) to detect the 
   button edge, how would you handle the fact that the interrupt might 
   fire 100 times in 5ms due to bouncing?

2. Vertical Counter Debouncing: How can you use bitwise math (a "vertical counter") 
   to debounce 8 or 16 buttons simultaneously in a single register?

3. Active Low vs. Active High: If your button has a pull-up resistor, 
   what is the GPIO level when the button is NOT pressed?

4. The "Long Press": How would you modify your FSM to distinguish 
   between a "short click" and a "3-second long press"?
*/