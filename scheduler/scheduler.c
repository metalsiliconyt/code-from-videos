#include <stdint.h>
#include <stdio.h>

// A bitmask where Bit 31 is the highest priority, Bit 0 is the lowest.
volatile uint32_t ready_tasks_bitmap = 0;

// 1. Mark a task as "Ready to Run"
void set_task_ready(uint8_t priority) {
    if (priority < 32)
    {
        ready_tasks_bitmap |= (1UL << priority);
    }
}

// 2. Mark a task as "Finished/Waiting"
void clear_task_ready(uint8_t priority) {
    if (priority < 32)
    {
        ready_tasks_bitmap &= ~(1UL << priority);
    }
}

// 3. THE MAGIC: Find the highest priority task in O(1)
int8_t get_highest_priority_task(void) {
    if (ready_tasks_bitmap < 0)
    {
        return -1;
    }

    uint32_t leading_zeros = __builtin_clz(ready_tasks_bitmap);

    return (31 - leading_zeros);
}

int main() {
    set_task_ready(5);
    set_task_ready(12);
    set_task_ready(2);
    //clear_task_ready(12);

    printf("Highest Priority Ready: %d\n", get_highest_priority_task()); 
    // Output: 21
    
    return 0;
}