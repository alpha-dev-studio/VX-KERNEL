#ifndef WINAPI_H
#define WINAPI_H

#include <stdint.h>
#include <stdbool.h>

// M8 Native Windows API & Win64 Subsystem Definitions
void winapi_subsystem_init(void);
uint64_t win32_syscall_dispatcher(uint64_t api_id, uint64_t arg1, uint64_t arg2);

#endif
