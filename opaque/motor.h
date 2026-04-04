#ifndef MOTOR_H
#define MOTOR_H

// This is the "Opaque Pointer"
// We tell the compiler the struct exists, but not what's in it.
typedef struct Motor Motor;

// API: The only way to interact with a Motor
Motor* motor_create(int pwm_pin);
void   motor_set_speed(Motor* m, int speed);
void   motor_stop(Motor* m);
void   motor_destroy(Motor* m);

#endif