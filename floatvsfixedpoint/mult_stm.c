#include <stdint.h>

typedef int32_t fixed_t;
// Q8.24 (8 bits for whole numbers, 24 for decimals)
#define Q_SHIFT 24
#define TO_FIXED(f) ((fixed_t)((f) * (1L << Q_SHIFT)))

// Software Emulation: The M0+ must call __aeabi_fmul (Very Heavy)
__attribute__((noinline)) float unpredictable_fmul(float a, float b) {
    return a * b;
}

// Hardware Accelerated: The M0+ uses its 32-bit multiplier (Very Fast)
__attribute__((noinline)) fixed_t unpredictable_xmul(fixed_t a, fixed_t b) {
    // STM32L031 has a hardware multiplier that handles this efficiently
    return (fixed_t)(((int64_t)a * b) >> Q_SHIFT);
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    // We can do more iterations here because multiplication is faster than division
    const int iterations = 1000;
    
    volatile float f_val = 1.001f;
    volatile float f_gain = 1.0001f;
    
    volatile fixed_t x_val = TO_FIXED(1.001f);
    volatile fixed_t x_gain = TO_FIXED(1.0001f);

    Serial.println("--- STM32L0 (Cortex-M0+) Multiplier Test ---");

    // --- FLOAT MULTIPLICATION ---
    uint32_t t1 = micros();
    for(int i=0; i < iterations; i++) {
        f_val = unpredictable_fmul(f_val, f_gain);
    }
    uint32_t t2 = micros();

    // --- FIXED MULTIPLICATION ---
    uint32_t t3 = micros();
    for(int i=0; i < iterations; i++) {
        x_val = unpredictable_xmul(x_val, x_gain);
    }
    uint32_t t4 = micros();

    uint32_t f_time = t2 - t1;
    uint32_t x_time = t4 - t3;

    Serial.print("Float Mult Time: "); Serial.print(f_time); Serial.println(" us");
    Serial.print("Fixed Mult Time: "); Serial.print(x_time); Serial.println(" us");
    
    if (x_time > 0) {
        float speedup = (float)f_time / (float)x_time;
        Serial.print("Result: Fixed Point is ");
        Serial.print(speedup, 2);
        Serial.println("x faster!");
    }

    Serial.print("Final Float: "); Serial.println(f_val, 6);
    Serial.print("Final Fixed: "); Serial.println((float)x_val / (1 << Q_SHIFT), 6);
}

void loop() {}