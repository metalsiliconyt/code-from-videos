#include <stdio.h>

// Define the function signature for a sensor read
typedef int (*read_fptr)(void);

// A struct to act as our "Interface"
typedef struct {
    read_fptr read;
    const char* name;
} SensorInterface;

// Hardware-specific implementations
int setup_temp_sensor(void) { return 25; } // Imagine I2C logic here

int main() {
    // Manually "hooking up" the function pointers
    SensorInterface temp_sensor = { .read = setup_temp_sensor, .name = "LM75" };
    
    // Usage in the app
    printf("Sensor setup for %s says: %d\n", temp_sensor.name, temp_sensor.read());
    
    return 0;
}