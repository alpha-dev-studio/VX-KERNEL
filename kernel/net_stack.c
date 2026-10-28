#include "net_stack.h"

void net_stack_init(void) {
    // Initialize network interface bindings, ARP cache, and socket tables
}

bool net_send_ipv4_packet(uint32_t dest_ip, uint8_t protocol, uint8_t *payload, uint16_t size) {
    (void)dest_ip;
    (void)protocol;
    (void)payload;
    (void)size;
    // Construct IPv4 header and hand off to network driver ring buffer
    return true;
}
