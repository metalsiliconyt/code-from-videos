/* --- sensor_driver.c --- */
#include "after.h"
#include <stddef.h>
#include <stdbool.h>

/* ✅ PRO TACTIC 4 (Continued): X-Macros
 * Expand the exact same list into an array of strings for logging.
 */
#define X_STR(VAL, STR) STR,
static const char* const SENSOR_ERR_STRINGS[] = {
    SENSOR_ERRORS(X_STR)
};

/* ✅ PRO TACTIC 5: Defensive Programming (Union Type Punning)
 * Instead of compiler-dependent bit-fields, we use a union to overlay 
 * a structured payload over a raw 32-bit hardware word, parsed with explicit masks.
 */
#define SENSOR_READY_MASK (1UL << 7)
#define SENSOR_FAULT_MASK (1UL << 6)

typedef union {
    uint32_t raw_word;
    struct {
        uint8_t payload[4]; // Guarantees exact byte alignment without padding issues
    } bytes;
} SensorPacket_t;

/* ✅ PRO TACTIC 3: Linker Script Section Placement & DMA
 * We place this buffer in a specific, non-cacheable RAM section defined in 
 * our linker script (e.g., .dma_ram). The DMA hardware writes directly here.
 * ✅ PRO TACTIC 2: Strategic Volatile
 * We mark it volatile because the hardware (DMA) changes it outside the CPU's knowledge.
 */
__attribute__((section(".dma_ram"), aligned(4)))
static volatile uint32_t dma_rx_buffer[16];

/* ✅ PRO TACTIC 1 (Continued): The "Private" Struct
 * The actual definition is hidden in the .c file. 
 */
struct SensorCore_t {
    volatile uint32_t* hw_base; 
    uint32_t is_locked; // For thread safety
};

// Singleton instance hidden from the rest of the program
static struct SensorCore_t instance = {
    .hw_base = (volatile uint32_t*)0x40013000,
    .is_locked = 0
};

/* ✅ PRO TACTIC 2: Strategic Inline
 * A fast, inline atomic lock to ensure thread safety without the overhead 
 * of a full OS mutex if we are in a bare-metal RTOS environment.
 */
static inline bool Atomic_TryLock(volatile uint32_t* lock) {
    // Uses GCC built-in for hardware atomic operations (e.g., LDREX/STREX on ARM)
    return (__sync_val_compare_and_swap(lock, 0, 1) == 0);
}

static inline void Atomic_Unlock(volatile uint32_t* lock) {
    __sync_lock_release(lock);
}

// --- Public API Implementations ---
SensorHandle Sensor_Init(void) {
    /* * 1. Enable Clock for DMA Peripheral
     * In production, you must power up the DMA controller before touching its registers.
     */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* * 2. Configure the DMA Channel
     * CPAR (Peripheral Address): The "Source" (The Sensor's Data Register).
     * CMAR (Memory Address): The "Destination" (Our .dma_ram buffer).
     */
    DMA1_Channel1->CPAR = (uint32_t)&(SPI1->DR);      // Address of the SPI Data Register
    DMA1_Channel1->CMAR = (uint32_t)dma_rx_buffer;    // Address of our RAM buffer

    /* * 3. Set the Number of Data items to transfer (16 words)
     */
    DMA1_Channel1->CNDTR = 16;

    /* * 4. Configure the Control Register (CCR)
     * - MINC: Memory Increment Mode (Move to the next array index after each write).
     * - PSIZE/MSIZE: 32-bit transfer size (matches our uint32_t buffer).
     * - CIRC: Circular mode (Start back at index 0 once 16 words are filled).
     * - EN: Enable the channel.
     */
    DMA1_Channel1->CCR = DMA_CCR_MINC  | 
                         DMA_CCR_MSIZE_1 | // 32-bit memory
                         DMA_CCR_PSIZE_1 | // 32-bit peripheral
                         DMA_CCR_CIRC    | 
                         DMA_CCR_EN;

    /* * 5. Trigger SPI to request DMA
     * We tell the SPI peripheral: "Don't wait for the CPU; send a signal to DMA 
     * every time you have a new RX byte."
     */
    SPI1->CR2 |= SPI_CR2_RXDMAEN;

    return &instance;
}

const char* Sensor_GetErrorString(SensorError_t err) {
    return SENSOR_ERR_STRINGS[err];
}

SensorError_t Sensor_ProcessDMA(SensorHandle handle, uint8_t* out_buffer) {
    if (handle == NULL || out_buffer == NULL) return SENSOR_ERR_BUSY;

    // 1. Thread-safety check
    if (!Atomic_TryLock(&handle->is_locked)) {
        return SENSOR_ERR_BUSY; 
    }

    /* 2. Calculate the "Live" Hardware Index
     * CNDTR counts DOWN (16 -> 15 -> ... -> 1 -> 0).
     * If CNDTR is 16, it's about to write to index 0.
     * If CNDTR is 15, it just wrote to index 0 and is moving to index 1.
     */
    uint32_t remaining = DMA1_Channel1->CNDTR;
    
    // We want the LAST completed write. 
    // If remaining == 16, the last write was at index 15 (end of previous loop).
    uint32_t latest_idx = (remaining == 16) ? 15 : (16 - remaining - 1);

    /* 3. The "Glass Box" Read
     * We grab the raw 32-bit word from the calculated hardware index.
     * Because dma_rx_buffer is 'volatile', the CPU fetches fresh from RAM.
     */
    SensorPacket_t packet;
    packet.raw_word = dma_rx_buffer[latest_idx]; 

    // 4. Defensive Masking (Pro Tactic 5)
    if (packet.raw_word & SENSOR_FAULT_MASK) {
        Atomic_Unlock(&handle->is_locked);
        return SENSOR_ERR_TIMEOUT;
    }

    // 5. Safe Extraction via Union Type Punning
    // Extract the specific data byte (e.g., Payload Byte 1)
    out_buffer[0] = packet.bytes.payload[1]; 

    Atomic_Unlock(&handle->is_locked);
    return SENSOR_OK;
}