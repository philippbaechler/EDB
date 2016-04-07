#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "PE_Types.h"

#define COL_CDATAL 0x14
#define COL_CDATAH 0x15
#define COL_RDATAL 0x16
#define COL_RDATAH 0x17
#define COL_GDATAL 0x18
#define COL_GDATAH 0x19
#define COL_BDATAL 0x1A
#define COL_BDATAH 0x1B

#define COL_I2C_ADDR (0x29) /* already definde in properties */

void COL_Init();
bool COL_RightContainer();
uint16_t COL_Process();

#endif /* COLORSENSOR_H */
