typedef struct {
    uint8_t data[64];
    bool is_used;
} MessageBlock;

#define POOL_SIZE 10
static MessageBlock msg_pool[POOL_SIZE];

MessageBlock* allocate_block() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (!msg_pool[i].is_used) {
            msg_pool[i].is_used = true;
            return &msg_pool[i];
        }
    }
    return NULL; // Predictable failure
}