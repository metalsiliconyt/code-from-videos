#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

typedef int32_t fixed_t;
#define Q_SHIFT 24
#define TO_FIXED(f) ((fixed_t)((f) * (1L << Q_SHIFT)))

// Prevent the compiler from optimizing out the math
__attribute__((noinline)) float unpredictable_fmul(float a, float b) {
    return a * b;
}

__attribute__((noinline)) fixed_t unpredictable_xmul(fixed_t a, fixed_t b) {
    return (fixed_t)(((int64_t)a * b) >> Q_SHIFT);
}

void app_main(void) {
    // Small delay to ensure the serial monitor is ready
    vTaskDelay(pdMS_TO_TICKS(500));

    printf("\n--- ESP32-S3 ESP-IDF Benchmark ---\n");

    const int iterations = 1000; 
    volatile float f_val = 1.001f;
    volatile float f_gain = 1.0001f;
    volatile fixed_t x_val = TO_FIXED(1.001f);
    volatile fixed_t x_gain = TO_FIXED(1.0001f);

    // --- FLOAT BENCHMARK ---
    int64_t t1 = esp_timer_get_time(); // Microseconds since boot
    for(int i=0; i < iterations; i++) {
        f_val = unpredictable_fmul(f_val, f_gain);
    }
    int64_t t2 = esp_timer_get_time();

    // --- FIXED BENCHMARK ---
    int64_t t3 = esp_timer_get_time();
    for(int i=0; i < iterations; i++) {
        x_val = unpredictable_xmul(x_val, x_gain);
    }
    int64_t t4 = esp_timer_get_time();

    // Results calculation
    uint32_t f_time = (uint32_t)(t2 - t1);
    uint32_t x_time = (uint32_t)(t4 - t3);

    printf("Iterations: %d\n", iterations);
    printf("Float Mult Total: %lu us\n", f_time);
    printf("Fixed Mult Total: %lu us\n", x_time);
    
    if (x_time > 0) {
        float speedup = (float)f_time / (float)x_time;
        printf("Result: Fixed Point is %.2fx faster/slower than Float.\n", speedup);
    }

    printf("Final Float: %f\n", f_val);
    printf("Final Fixed: %f\n", (float)x_val / (float)(1L << Q_SHIFT));
}