#include "motor.h"
#include <stdlib.h>
#include <stdio.h>

// The "Secret" definition
struct Motor {
    int pwm_pin;
    int current_speed;
    float safety_threshold; // Internal value user shouldn't touch!
};

Motor* motor_create(int pwm_pin) {
    Motor* m = malloc(sizeof(struct Motor));
    if (m) {
        m->pwm_pin = pwm_pin;
        m->current_speed = 0;
        m->safety_threshold = 0.95f; 
        printf("Motor initialized on Pin %d\n", pwm_pin);
    }
    return m;
}

void motor_set_speed(Motor* m, int speed) {
    if (speed > 100) speed = 100; // Safety logic encapsulated!
    m->current_speed = speed;
    printf("Motor speed set to %d%%\n", m->current_speed);
}

void motor_destroy(Motor* m) {
    free(m);
    printf("Motor resource cleaned up.\n");
}