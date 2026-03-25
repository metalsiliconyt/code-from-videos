/* --- bad_sensor.c --- */
#include <stdint.h>
#include <stdbool.h>

// ❌ BAD: Global arrays. Any file can modify this memory.
uint32_t sensor_raw_data[16]; 
int current_state = 0; 

// ❌ BAD: Hardcoded magic numbers for hardware addresses
#define SENSOR_SPI_DR 0x4001300C 
#define SPI_SR 0x40013008

void Read_Sensor_Blocking() {
    // ❌ BAD: Polling a magic number without a timeout. 
    // If the hardware fails, this loop hangs the entire system forever.
    while ( (*(volatile uint32_t*)SPI_SR) & 0x02 == 0 ); 
    
    // ❌ BAD: CPU is wasting cycles copying data byte-by-byte
    for(int i = 0; i < 16; i++) {
        sensor_raw_data[i] = *(volatile uint32_t*)SENSOR_SPI_DR;
    }
    
    // ❌ BAD: Non-portable compiler-dependent bit-fields for parsing
    struct {
        uint8_t id : 4;
        uint8_t val : 4;
    } parsed_data;
    
    parsed_data.id = sensor_raw_data[0] & 0x0F; // Magic masks everywhere
}