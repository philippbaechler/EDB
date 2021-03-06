#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "PE_Types.h"
#include "BLUETOOTH.h"

typedef struct colors{
	uint16_t clearValue;
	uint16_t redValue;
	uint16_t greenValue;
	uint16_t blueValue;
}colors_t;

// slave address
#define TCS34725_ADDRESS          0x29

//addresses for the registers
#define EnablesStatesAndInterrupts 					0x80

#define RGBTime										0x81

#define WaitTime									0x83
#define ClearInterruptLowThresholdLowByte			0x84
#define ClearInterruptLowThresholdHighByte			0x85
#define ClearInterruptHighThresholdLowByte			0x86
#define ClearInterruptHighThresholdHighByte			0x87

#define InterruptPersistanceFilter					0x8C
#define Configuration								0x8D

#define GainControlRegister							0x8F

// Startup settings
#define TCS34725_COMMAND_BIT      						0xA1

#define RGBTime_init									0xC0
#define WaitTime_init									0xF0
#define ClearInterruptLowThresholdLowByte_init			0x00
#define ClearInterruptLowThresholdHighByte_init			0x00
#define ClearInterruptHighThresholdLowByte_init			0x00
#define ClearInterruptHighThresholdHighByte_init		0x00
#define InterruptPersistanceFilter_init					0x10
#define Configuration_init								0x00
#define GainControlRegister_init						0x00

// enable register
#define CommandCodeEnable							0x80
#define Enable_init									0x0b

#define ClearValuesArraySize	5

void COL_TurnOnLED();
void COL_TurnOffLED();
uint16_t COL_ReadClear();
uint16_t COL_ReadRed();
uint16_t COL_ReadGreen();
uint16_t COL_ReadBlue();
void COL_ReadColors();
bool COL_RightContainer();
bool COL_ClearReachedPeak();
uint8_t COL_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);
void COL_Init();

#endif /* COLORSENSOR_H */
