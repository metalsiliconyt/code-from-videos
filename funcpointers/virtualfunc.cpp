#include <iostream>

// 1. The Base Class (The Interface)
class Peripheral {
protected:
    const char* name; // Pointer to a string in Flash memory

public:
    // Base constructor to store the name
    Peripheral(const char* n) : name(n) {}

    virtual int read() = 0;   // Pure virtual function
    virtual ~Peripheral() {}  // Always include a virtual destructor!

    // Helper to get the name (doesn't need to be virtual)
    const char* getName() const { return name; }
};

// 2. Concrete Implementation
class TempSensor : public Peripheral {
public:
    // Call the base constructor with the specific name
    TempSensor() : Peripheral("Internal Temp Sensor") {}

    int read() override {
        // Imagine direct register access here: return ADC1->DR;
        return 55;
    }
};

// 3. The Dispatcher
void system_read(Peripheral* dev) {
    // Accessing both the data and the identity from the object
    std::cout << "Reading from " << dev->getName() << ": " << dev->read() << std::endl;
}

int main() {
    TempSensor mySensor;
    system_read(&mySensor);
    return 0;
}