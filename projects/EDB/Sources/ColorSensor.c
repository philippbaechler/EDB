#include "ColorSensor.h"
#include "PE_Types.h"
#include "RGB_SENSOR.h"
#include "WAIT.h"
#include "PE_Error.h"


void COL_Init(){

}

bool COL_RightContainer(){
	return FALSE;
}

uint16_t COL_Process(){

	uint8_t buf[2];

//	byte buf;

	byte res;

	buf[0] = 0;
	buf[1] = 0;


	byte addr = COL_BDATAL;

	res = RGB_SENSOR_ReadAddress(COL_I2C_ADDR, (uint8_t*)&addr, sizeof(addr), (uint8_t*)&buf[0], sizeof(buf));

//	res = RGB_SENSOR_ReadByteAddress8(COL_I2C_ADDR, addr, &buf);

	uint16_t r = (uint16_t)((buf[0]<<8)|buf[1]);

	 return r;
}
