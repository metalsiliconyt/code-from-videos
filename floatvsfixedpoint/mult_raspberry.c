#include <stdint.h>

typedef int32_t fixed_t;
#define Q_SHIFT 24
#define TO_FIXED(f) ((fixed_t)((f) * (1L << Q_SHIFT)))

// Force no-inline so we can actually time the function overhead
__attribute__((noinline)) float unpredictable_fmul(float a, float b) {
    return a * b;
}

__attribute__((noinline)) fixed_t unpredictable_xmul(fixed_t a, fixed_t b) {
    return (fixed_t)(((int64_t)a * b) >> Q_SHIFT);
}

void setup() {
    // 1. Initialize Serial at a standard baud rate
    Serial1.begin(115200);

    Serial1.println("--- Raspberry Pi Pico Benchmark Connected ---");

    const int iterations = 1000;
    volatile float f_val = 1.001f;
    volatile float f_gain = 1.0001f;
    volatile fixed_t x_val = TO_FIXED(1.001f);
    volatile fixed_t x_gain = TO_FIXED(1.0001f);

    // --- FLOAT BENCHMARK ---
    uint32_t t1 = micros();
    for(int i=0; i < iterations; i++) {
        f_val = unpredictable_fmul(f_val, f_gain);
    }
    uint32_t t2 = micros();

    // --- FIXED BENCHMARK ---
    uint32_t t3 = micros();
    for(int i=0; i < iterations; i++) {
        x_val = unpredictable_xmul(x_val, x_gain);
    }
    uint32_t t4 = micros();

    // Results calculation
    uint32_t f_time = t2 - t1;
    uint32_t x_time = t4 - t3;

    Serial1.print("Float Mult Time: "); Serial1.print(f_time); Serial1.println(" us");
    Serial1.print("Fixed Mult Time: "); Serial1.print(x_time); Serial1.println(" us");
    
    if (x_time > 0) {
        float speedup = (float)f_time / (float)x_time;
        Serial1.print("Result: Fixed Point is ");
        Serial1.print(speedup, 2);
        Serial1.println("x faster.");
    }

    Serial1.print("Final Float: "); Serial1.println(f_val, 6);
    Serial1.print("Final Fixed: "); Serial1.println((float)x_val / (float)(1L << Q_SHIFT), 6);
}

void loop() {}