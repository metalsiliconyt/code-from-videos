/* --- sensor_driver.h --- */
#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdint.h>

/* ✅ PRO TACTIC 4: X-Macros
 * Define our error codes ONCE. We will use this exact list in the .c file 
 * to auto-generate our string-conversion function. No more copy-paste mismatch.
 */
#define SENSOR_ERRORS(X) \
    X(SENSOR_OK, "Operation Successful") \
    X(SENSOR_ERR_TIMEOUT, "DMA Transfer Timeout") \
    X(SENSOR_ERR_BUSY, "Hardware Lock Contention")

// Expand the X-Macro into an Enum
#define X_ENUM(VAL, STR) VAL,
typedef enum {
    SENSOR_ERRORS(X_ENUM)
} SensorError_t;

/* ✅ PRO TACTIC 1: Opaque Pointers
 * The user of this module only gets a pointer to an incomplete struct.
 * They CANNOT access the internal hardware registers or state variables directly.
 */
typedef struct SensorCore_t* SensorHandle;

// Public API
SensorHandle Sensor_Init(void);
SensorError_t Sensor_ProcessDMA(SensorHandle handle, uint8_t* out_buffer);
const char* Sensor_GetErrorString(SensorError_t err);

#endif // SENSOR_DRIVER_H