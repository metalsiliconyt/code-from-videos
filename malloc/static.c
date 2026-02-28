#define MAX_BUFFER_SIZE 512

// Allocated in the .bss section at compile time
static uint8_t sensor_buffer[MAX_BUFFER_SIZE];

void safe_process(void) {
    // No allocation needed, no fragmentation possible
    update_buffer(sensor_buffer);
}