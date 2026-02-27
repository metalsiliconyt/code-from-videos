typedef enum {
    STATE_IDLE,
    STATE_HEATING,
    STATE_BREWING,
    STATE_ERROR
} State_t;

typedef enum {
    EVENT_START_PRESSED,
    EVENT_TEMP_REACHED,
    EVENT_BREW_COMPLETE,
    EVENT_OUT_OF_WATER
} Event_t;

// Structure to represent the system status
typedef struct {
    State_t currentState;
    int waterLevel; // 0 to 100
    int currentTemp; // in Celsius
} CoffeeMachine;

// Function to process events
void FSM_Update(CoffeeMachine* m, Event_t e);