#include "motor.h"

int main() {
    // Create the motor handle
    Motor* my_motor = motor_create(12);

    // Use the public API (Works perfectly)
    motor_set_speed(my_motor, 50);

    // TRY TO BREAK IT 
    //my_motor->safety_threshold = 2.0f; 

    motor_destroy(my_motor);
    return 0;
}