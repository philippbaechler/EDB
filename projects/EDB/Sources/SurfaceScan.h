#ifndef SURFACESCAN_H
#define SURFACESCAN_H

#include "PE_Types.h"
#include "BLUETOOTH.h"

#define percentRisingEdge 1.2
#define percentFallingEdge 0.8
#define containerLengthMinSteps 250
#define containerLengthMaxSteps 400
#define waitBetweenMeasurementsMS 5
#define measurementsBufferSize 10

bool SCN_IsAContainer();
uint8_t SCN_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);

#endif /* SURFACESCAN_H */
