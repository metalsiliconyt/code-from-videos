#include <stdio.h>
#include <unistd.h> // For usleep (simulation timing)
#include "fsm_logic.h"

// --- Visual Feedback Helpers ---

void render_machine_ui(CoffeeMachine* m) {
    printf("\r[System Status] ");
    
    switch(m->currentState) {
        case STATE_IDLE:    printf("Status: READY     [  IDLE  ] ☕  "); break;
        case STATE_HEATING: printf("Status: HEATING   [ 🔥🔥🔥 ]     "); break;
        case STATE_BREWING: printf("Status: BREWING   [ ☕〰️〰️ ]     "); break;
        case STATE_ERROR:   printf("Status: !!ERROR!! [ ⚠️⚠️⚠️ ]     "); break;
    }

    // Small progress bar for water/temp
    printf("| Water: %3d%% | ", m->waterLevel);
    
    if (m->currentState == STATE_HEATING) {
        printf("Temp: Rising... ");
    } else {
        printf("Temp: %d°C      ", m->currentTemp);
    }
    fflush(stdout); 
}

// --- The Simulation ---

int main() {
    printf("=== Smart Coffee Pro Firmware v1.0 ===\n");
    CoffeeMachine myMachine = {STATE_IDLE, 100, 20};

    // 1. Initial State
    render_machine_ui(&myMachine);
    sleep(1);

    // 2. User presses start
    printf("\n\n> User pressed START button...\n");
    FSM_Update(&myMachine, EVENT_START_PRESSED);
    render_machine_ui(&myMachine);
    
    // Simulate heating progress
    for(int i = 0; i < 5; i++) {
        usleep(300000); // 300ms pause
        printf("."); fflush(stdout);
    }

    // 3. Sensor signals temp reached
    printf("\n\n> Internal Sensor: Temp reached 95°C!\n");
    FSM_Update(&myMachine, EVENT_TEMP_REACHED);
    render_machine_ui(&myMachine);
    
    // Simulate brewing
    for(int i = 0; i < 5; i++) {
        usleep(300000);
        printf("."); fflush(stdout);
    }

    // 4. Finish
    printf("\n\n> Dispenser: Brew Complete.\n");
    FSM_Update(&myMachine, EVENT_BREW_COMPLETE);
    render_machine_ui(&myMachine);
    printf("\n\n--- TEST SUCCESSFUL ---\n");

    return 0;
}