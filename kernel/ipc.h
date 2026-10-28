#ifndef IPC_H
#define IPC_H

#include <stdint.h>
#include <stdbool.h>

struct ipc_message {
    uint64_t sender_pid;
    uint64_t recipient_pid;
    uint64_t msg_type;
    uint64_t data_size;
    uint8_t payload[256];
};

bool ipc_send(struct ipc_message *msg);
bool ipc_receive(uint64_t recipient_pid, struct ipc_message *msg);

#endif
