#include "RGB_SENSOR.h"
#include "ColorSensor.h"
#include "Serial.h"

typedef struct colors{
	uint16_t clearValue;
	uint16_t redValue;
	uint16_t greenValue;
	uint16_t blueValue;
}colors_t;

colors_t colors;

uint8_t RGBTimeArr[] = {RGBTime, RGBTime_init};
uint8_t WaitTimeArr[] = {WaitTime, WaitTime_init};
uint8_t CILTLBArr[] = {ClearInterruptLowThresholdLowByte, ClearInterruptLowThresholdLowByte_init};
uint8_t CILTHBArr[] = {ClearInterruptHighThresholdLowByte, ClearInterruptHighThresholdLowByte_init};
uint8_t CIHTLBArr[] = {ClearInterruptHighThresholdLowByte, ClearInterruptHighThresholdLowByte_init};
uint8_t CIHTHBArr[] = {ClearInterruptHighThresholdHighByte, ClearInterruptHighThresholdHighByte_init};
uint8_t InterruptPersistanceFilterArr[] = {InterruptPersistanceFilter, InterruptPersistanceFilter_init};
uint8_t ConfigurationArr[] = {Configuration, Configuration_init};
uint8_t GainControlRegisterArr[] = {GainControlRegister, GainControlRegister_init};



// enable register
#define CommandCodeEnable							0x80
#define Enable_init									0x0b

uint8_t enable[] = {CommandCodeEnable, Enable_init};
#define lengthOfEnable			2



// register addresses for reading
uint8_t DeviceID								=	0x92;
uint8_t DiviceStauts							=	0x93;
uint8_t ClearADClowDataRegister					=	0x94;
uint8_t ClearADChighDataRegister				=	0x95;
uint8_t RedADClowDataRegister					=	0x96;
uint8_t RedADChighDataRegister					=	0x97;
uint8_t GreenADClowDataRegister					=	0x98;
uint8_t GreenADChighDataRegister				=	0x99;
uint8_t BlueADClowDataRegister					=	0x9A;
uint8_t BlueADChighDataRegister					=	0x9B;


uint16_t COL_ReadClear(){
	uint16_t clearResult = 0;
	uint8_t clear[2];

	RGB_SENSOR_WriteBlock(&ClearADClowDataRegister, 1, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_ReadBlock((uint8_t*)&clear, 2,RGB_SENSOR_SEND_STOP);

	clearResult = (uint8_t)clear[1] << 8;
	clearResult |= clear[0];

	return clearResult;
}

uint16_t COL_ReadRed(){
	uint16_t redResult = 0;
	uint8_t red[2];

	RGB_SENSOR_WriteBlock(&RedADClowDataRegister, 1, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_ReadBlock((uint8_t*)&red, 2,RGB_SENSOR_SEND_STOP);

	redResult = (uint8_t)red[1] << 8;
	redResult |= red[0];

	return redResult;
}

uint16_t COL_ReadGreen(){
	uint16_t greenResult = 0;
	uint8_t green[2];

	RGB_SENSOR_WriteBlock(&GreenADClowDataRegister, 1, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_ReadBlock((uint8_t*)&green, 2,RGB_SENSOR_SEND_STOP);

	greenResult = (uint8_t)green[1] << 8;
	greenResult |= green[0];

	return greenResult;
}

uint16_t COL_ReadBlue(){
	uint16_t blueResult = 0;
	uint8_t blue[2];

	RGB_SENSOR_WriteBlock(&BlueADClowDataRegister, 1, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_ReadBlock((uint8_t*)&blue, 2,RGB_SENSOR_SEND_STOP);

	blueResult = (uint8_t)blue[1] << 8;
	blueResult |= blue[0];

	return blueResult;
}

void COL_ReadColors(){

	colors.clearValue = COL_ReadClear();

	WAIT_Waitms(1);

	colors.redValue = COL_ReadRed();

	WAIT_Waitms(1);

	colors.greenValue = COL_ReadGreen();

	WAIT_Waitms(1);

	colors.blueValue = COL_ReadBlue();

	WAIT_Waitms(1);

//	uint16_t clearRes = 0;
//	clearRes = (uint8_t)clear[1] << 8;
//	clearRes |= clear[0];

//	uint16_t redRes = 0;
//	redRes = (uint8_t)red[1] << 8;
//	redRes |= red[0];

//	uint16_t greenRes = 0;
//	greenRes = (uint8_t)green[1] << 8;
//	greenRes |= green[0];

//	uint16_t blueRes = 0;
//	blueRes = (uint8_t)blue[1] << 8;
//	blueRes |= blue[0];

	SER_SendString("c: ");		// debugging
	SER_SendUint16(colors.clearValue);

	SER_SendString("  r: ");
	SER_SendUint16(colors.redValue);

	SER_SendString("  g: ");
	SER_SendUint16(colors.greenValue);

	SER_SendString("  b: ");
	SER_SendUint16(colors.blueValue);
	SER_SendNewLine();
}

void COL_Init(){

	// select slave by address
	RGB_SENSOR_SelectSlave(TCS34725_ADDRESS);

	// set the startup settings
	RGB_SENSOR_WriteBlock(&RGBTimeArr, 2, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_WriteBlock(&WaitTimeArr, 2, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_WriteBlock(&CILTLBArr, 2, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_WriteBlock(&CILTHBArr, 2, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_WriteBlock(&CIHTLBArr, 2, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_WriteBlock(&CIHTHBArr, 2, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_WriteBlock(&InterruptPersistanceFilterArr, 2, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_WriteBlock(&ConfigurationArr, 2, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_WriteBlock(&GainControlRegisterArr, 2, RGB_SENSOR_DO_NOT_SEND_STOP);

	// enable the device
	RGB_SENSOR_WriteBlock(enable, lengthOfEnable, RGB_SENSOR_SEND_STOP);
}

