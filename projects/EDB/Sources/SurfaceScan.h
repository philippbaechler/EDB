#ifndef SURFACESCAN_H
#define SURFACESCAN_H

#include "PE_Types.h"
#include "BLUETOOTH.h"

#define percentRisingEdge 1.2
#define percentFallingEdge 0.8
#define containerLengthMinSteps 250 // = 29.5mm / 0.1178mm --> at the speed of 50 mm/s
#define containerLengthMaxSteps 400 // = 47mm / 0.1178mm
#define waitBetweenMeasurementsMS 10
#define measurementsBufferSize 20

bool SCN_IsAContainer();
uint8_t SCN_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);

#endif /* SURFACESCAN_H */
