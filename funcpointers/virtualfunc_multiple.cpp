#include <iostream>

class Sensor {
protected:
    const char* name;
public:
    Sensor(const char* n) : name(n) {}
    virtual void init() = 0;
    virtual int  read() = 0;
    virtual void reset() = 0;
    virtual ~Sensor() {}    
    const char* getName() const { return name; }
};

class TempSensor : public Sensor {
public:
    TempSensor() : Sensor("Temp(LM75)") {}
    void init() override { std::cout << "LM75: Init\n"; }
    int  read() override { return 25; } 
    void reset() override { std::cout << "LM75: Reset\n"; }
};

class AccelSensor : public Sensor {
public:
    AccelSensor() : Sensor("Accel(ADXL)") {}
    void init() override { std::cout << "ADXL: Init\n"; }
    int  read() override { return 10; } 
    void reset() override { std::cout << "ADXL: Reset\n"; }
};

// --- THE KEY CHANGE ---
// This function is "Blind." It doesn't know if it's getting a Temp or Accel.
// It is FORCED to check the VTable pointer in RAM.
void run_sensor_logic(Sensor* s) {
    std::cout << "Configuring " << s->getName() << std::endl;
    s->init();
    std::cout << "Read: " << s->read() << std::endl;
    s->reset();
}

int main() {
    TempSensor  temp;
    AccelSensor accel;

    // We pass them as pointers to the base class
    Sensor* sensors[] = { &temp, &accel };

    for(int i = 0; i < 2; ++i) {
        run_sensor_logic(sensors[i]);
    }

    return 0;
}