#ifndef SCHED_H
#define SCHED_H

#include <stdint.h>
#include <stdbool.h>

struct cpu_context {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t rip, cs, rflags, rsp, ss;
};

struct process {
    uint64_t pid;
    uint64_t cr3;
    uint64_t rsp;
    int state; // 0 = Ready, 1 = Running, 2 = Blocked, 3 = Terminated
};

void scheduler_init(void);
void schedule_tick(struct cpu_context *ctx);

#endif
