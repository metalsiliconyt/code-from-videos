#include <stdbool.h>
#include <stdio.h>

// Global flags - the hallmark of spaghetti code
bool is_running = false;
bool is_heating = false;
bool is_brewing = false;
bool has_error = false;
int water_level = 100;

void ProcessInput(int event) {
    // In spaghetti code, we often see a giant list of "if" checks
    // that are difficult to maintain or reason about.
    
    if (event == 0) { // START_PRESSED
        if (!is_running && !has_error) {
            if (water_level > 10) {
                is_running = true;
                is_heating = true;
                printf("Starting...\n");
            } else {
                has_error = true;
            }
        }
    } 
    
    if (event == 1) { // TEMP_REACHED
        // We have to check if we were actually heating first
        if (is_heating && !has_error) {
            is_heating = false;
            is_brewing = true;
            printf("Brewing now...\n");
        }
    }

    if (event == 2) { // BREW_COMPLETE
        if (is_brewing) {
            is_brewing = false;
            is_running = false;
            printf("Done.\n");
        }
    }
    
    if (event == 3) { // OUT_OF_WATER
        if (is_heating || is_brewing) {
            has_error = true;
            is_running = false;
            is_heating = false;
            is_brewing = false;
        }
    }
}

/*
The "Race Condition" Bug: In the spaghetti code, if is_heating and is_brewing both somehow become true 
(due to a bug elsewhere), the system enters an undefined state. 
In the FSM, currentState can only be one value at a time.

The "Add a Feature" Test: "If I want to add a 'Cleaning' mode, 
how many 'if' statements do I have to modify in the spaghetti code versus the FSM?" 
(Answer: In FSM, you just add one case).

The "Input Spam" Problem: If the user mashes the "Start" button while the machine is already brewing, 
the spaghetti code might restart the timer or glitch. The FSM naturally ignores 
EVENT_START_PRESSED while in STATE_BREWING because there is no case for it.
*/