#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "BLUETOOTH.h"
#include "PE_Types.h"

#define LowBatteryLight 30000 // set these values corresponding to the battery-voltage-monitor
#define LowBatteryBlink 25000
#define ShutOffVoltage 20000

uint8_t WDG_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);
void WDG_Init();

#endif /*WATCHDOG_H*/
