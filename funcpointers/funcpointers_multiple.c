#include <stdio.h>

typedef int  (*read_fptr)(void);
typedef void (*init_fptr)(void);
typedef void (*reset_fptr)(void);

typedef struct {
    const char* name;
    init_fptr   init;
    read_fptr   read;
    reset_fptr  reset;
} SensorInterface;

// Hardware logic
void init_temp()  { printf("LM75: Up\n"); }
int  read_temp()  { return 25; }
void reset_temp() { printf("LM75: Reset\n"); }

void init_accel()  { printf("ADXL: Up\n"); }
int  read_accel()  { return 10; }
void reset_accel() { printf("ADXL: Reset\n"); }

void run_sensor(SensorInterface* s) {
    printf("Configuring %s\n", s->name);
    s->init();
    printf("Value: %d\n", s->read());
    s->reset();
}

int main() {
    // 1. These structs MUST exist in RAM now.
    SensorInterface temp = { "Temp(LM75)", init_temp, read_temp, reset_temp };
    SensorInterface accel = { "Accel(ADXL)", init_accel, read_accel, reset_accel };

    // 2. The Pointer Array (The Manager)
    SensorInterface* sensors[] = { &temp, &accel };

    // 3. The Loop (Forces the compiler to use the addresses)
    for(int i = 0; i < 2; i++) {
        run_sensor(sensors[i]);
    }

    return 0;
}