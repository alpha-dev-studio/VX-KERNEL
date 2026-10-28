#ifndef DRIVERS_H
#define DRIVERS_H

#include <stdint.h>
#include <stdbool.h>

// M7 Comprehensive Embedded Driver Subsystems
void drivers_init_all(void);
bool ahci_driver_init(void);
bool nvme_driver_init(void);
bool intel_amd_nvidia_gpu_init(void);
bool net_wifi_driver_init(void);
bool usb_hid_driver_init(void);

#endif
