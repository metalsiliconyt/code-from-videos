#include "pid.h"
#include <stdlib.h>

struct PIDController {
    float kp, ki, kd;      // Tuning constants
    float integral;        // HIDE THIS: Internal accumulation
    float prev_error;      // HIDE THIS: Required for derivative
    float last_timestamp;  // HIDE THIS: Internal timing logic
};

PIDController* pid_create(float kp, float ki, float kd) {
    PIDController* pid = malloc(sizeof(struct PIDController));
    if (pid) {
        pid->kp = kp;
        pid->ki = ki;
        pid->kd = kd;
        pid->integral = 0.0f;
        pid->prev_error = 0.0f;
    }
    return pid;
}

float pid_compute(PIDController* pid, float setpoint, float actual) {
    float error = setpoint - actual;
    
    // Internal state updates
    pid->integral += error; 
    float derivative = error - pid->prev_error;
    pid->prev_error = error;

    return (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
}

void pid_reset(PIDController* pid) {
    // A safe way for the user to "zero out" the controller
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}

void pid_destroy(PIDController* pid) {
    free(pid);
}