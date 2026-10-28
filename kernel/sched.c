#include "sched.h"

static struct process current_proc;
static uint64_t next_pid = 1;

void scheduler_init(void) {
    current_proc.pid = next_pid++;
    current_proc.state = 1; // Running
}

void schedule_tick(struct cpu_context *ctx) {
    // Basic round-robin or cooperative scheduling placeholder
    (void)ctx;
}
