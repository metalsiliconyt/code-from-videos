#include "fsm_logic.h"

void FSM_Update(CoffeeMachine* m, Event_t e) {
    switch (m->currentState) {
        case STATE_IDLE:
            if (e == EVENT_START_PRESSED) {
                if (m->waterLevel > 10) m->currentState = STATE_HEATING;
                else m->currentState = STATE_ERROR;
            }
            break;

        case STATE_HEATING:
            if (e == EVENT_TEMP_REACHED) m->currentState = STATE_BREWING;
            else if (e == EVENT_OUT_OF_WATER) m->currentState = STATE_ERROR;
            break;

        case STATE_BREWING:
            if (e == EVENT_BREW_COMPLETE) m->currentState = STATE_IDLE;
            break;

        case STATE_ERROR:
            // Error requires a reset (simplified for this example)
            break;
    }
}