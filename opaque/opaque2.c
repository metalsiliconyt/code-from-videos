#include "pid.h"
#include <stdio.h>

int main() {
    // Setup the controller for a heater
    PIDController* heater_pid = pid_create(2.0f, 0.5f, 0.1f);

    float current_temp = 25.0f;
    float target_temp = 100.0f;

    // Control Loop
    for (int i = 0; i < 10; i++) {
        float power = pid_compute(heater_pid, target_temp, current_temp);
        printf("Step %d: Output Power = %.2f\n", i, power);
        current_temp += (power * 0.1f); // Simulate heating
    }

    // If we didn't use an opaque pointer, a junior dev might try:
    heater_pid->integral = 0; // This would cause a COMPILE ERROR.
    pid_reset(heater_pid);

    pid_destroy(heater_pid);
    return 0;
}