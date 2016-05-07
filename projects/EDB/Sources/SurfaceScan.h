#ifndef SURFACESCAN_H
#define SURFACESCAN_H

#include "PE_Types.h"
#include "BLUETOOTH.h"

bool SCN_IsAContainer();
uint8_t SCN_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);

#endif /* SURFACESCAN_H */
