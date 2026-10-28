#include "ipc.h"

#define MAX_QUEUE 32
static struct ipc_message message_queue[MAX_QUEUE];
static int queue_head = 0;
static int queue_tail = 0;

bool ipc_send(struct ipc_message *msg) {
    int next_tail = (queue_tail + 1) % MAX_QUEUE;
    if (next_tail == queue_head) {
        return false; // Queue full (back-pressure)
    }
    message_queue[queue_tail] = *msg;
    queue_tail = next_tail;
    return true;
}

bool ipc_receive(uint64_t recipient_pid, struct ipc_message *msg) {
    if (queue_head == queue_tail) {
        return false; // Queue empty
    }
    if (message_queue[queue_head].recipient_pid == recipient_pid) {
        *msg = message_queue[queue_head];
        queue_head = (queue_head + 1) % MAX_QUEUE;
        return true;
    }
    return false;
}
