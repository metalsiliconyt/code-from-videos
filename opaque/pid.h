#ifndef PID_H
#define PID_H

// The Opaque Pointer handle
typedef struct PIDController PIDController;

// Constructor and Destructor
PIDController* pid_create(float kp, float ki, float kd);
void           pid_destroy(PIDController* pid);

// Public API
void  pid_set_gains(PIDController* pid, float kp, float ki, float kd);
float pid_compute(PIDController* pid, float setpoint, float actual);
void  pid_reset(PIDController* pid); // Safely clears internal state

#endif