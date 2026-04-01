//
// Temp monitor device with with added writing to SD card
// (Concurrency requirement added)
//

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Shared resource handles
SemaphoreHandle_t xLogMutex;
QueueHandle_t xSensorQueue = NULL;

// Task Handles
TaskHandle_t xAuthTaskHandle = NULL;
TaskHandle_t xSensorTaskHandle = NULL;
TaskHandle_t xLoggingTaskHandle = NULL;

// This is NOT an RTOS task. It's a Hardware Vector.
void EXTI0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Use the "FromISR" version to signal the Auth task
    vTaskNotifyGiveFromISR(xAuthTaskHandle, &xHigherPriorityTaskWoken);

    // Forces the scheduler to switch to the Auth task IMMEDIATELY 
    // after the ISR finishes, rather than waiting for the next 1ms tick.
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Task Prototypes
void vAuthTask(void *pvParameters);
void vSensorTask(void *pvParameters);
void vLoggingTask(void *pvParameters);

void vAuthTask(void *pvParameters) {
    while (1) {
        // Task BLOCKS here (0% CPU usage) until the ISR calls NotifyGive
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); 

        // Now that we are awake, check the PIN
        if (get_pin_input()) {
            if (verify_pin()) {
                xTaskNotifyGive(xSensorTaskHandle); 
            }
        }
    }
}

void vSensorTask(void *pvParameters) {
    uint32_t ulNotifiedValue;
    while (1) {
        // Wait for AuthTask to "Unlock" us
        ulNotifiedValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        float temp = read_high_precision_sensor();
        // Send data to the Logger Task via a Queue
        xQueueSend(xSensorQueue, &temp, 0);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // Sample every second
    }
}

void vLoggingTask(void *pvParameters) {
    float received_temp;
    while (1) {
        // This blocks indefinitely until data arrives in the queue
        if (xQueueReceive(xSensorQueue, &received_temp, portMAX_DELAY)) {
            // SD Card writes are slow, but since this is low priority,
            // the AuthTask can interrupt (preempt) this mid-write!
            write_to_sd_card(received_temp);
        }
    }
}

int main(void) {
    // 1. Hardware Setup (Clocks, GPIO, UART, etc.)
    prvSetupHardware();

    // 2. Create the Queue (Storage for 10 floats)
    xSensorQueue = xQueueCreate(10, sizeof(float));

    if (xSensorQueue != NULL) {
        
        // 3. Create Tasks
        // Syntax: Task Function, Name, Stack Size (words), Params, Priority, Handle
        
        xTaskCreate(vAuthTask, "Auth", 256, NULL, 3, &xAuthTaskHandle);
        xTaskCreate(vSensorTask, "Sensor", 256, NULL, 2, &xSensorTaskHandle);
        xTaskCreate(vLoggingTask, "Logger", 512, NULL, 1, &xLoggingTaskHandle);

        // 4. Start the Scheduler
        // This is the point of no return. The "while(1)" loop of main is never reached.
        vTaskStartScheduler();
    }

    // Should never get here unless there is insufficient RAM
    for (;;);
    return 0;
}