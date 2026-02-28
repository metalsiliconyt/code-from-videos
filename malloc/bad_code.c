// DO NOT DO THIS IN PRODUCTION
void process_sensor_data(int size) {
    uint8_t *buffer = (uint8_t *)malloc(size * sizeof(uint8_t));
    
    if (buffer == NULL) {
        // In firmware, 'NULL' often means 'Game Over'
        system_reboot(); 
    }

    // Do work...
    free(buffer);
}