#ifndef SURFACESCAN_H
#define SURFACESCAN_H

#include "PE_Types.h"
#include "BLUETOOTH.h"

#define percentRisingEdge 1.2
#define percentFallingEdge 0.8
#define containerLengthMinSteps 382 // 45mm / 0.1178mm
#define containerLengthMaxSteps 467 // 55mm / 0.1178mm
#define waitBetweenMeasurementsMS 100

bool SCN_IsAContainer();
uint8_t SCN_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);

#endif /* SURFACESCAN_H */
