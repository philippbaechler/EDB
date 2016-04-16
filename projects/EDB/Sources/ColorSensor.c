#include "RGB_SENSOR.h"
#include "ColorSensor.h"
#include "Serial.h"


//// slave address
//#define TCS34725_ADDRESS          0x29
//
////addresses for the registers
//#define EnablesStatesAndInterrupts 					0x80
//
//#define RGBTime										0x81
//
//#define WaitTime									0x83
//#define ClearInterruptLowThresholdLowByte			0x84
//#define ClearInterruptLowThresholdHighByte			0x85
//#define ClearInterruptHighThresholdLowByte			0x86
//#define ClearInterruptHighThresholdHighByte			0x87
//
//#define InterruptPersistanceFilter					0x8C
//#define Configuration								0x8D
//
//#define GainControlRegister							0x8F
//
//// Startup settings
//#define TCS34725_COMMAND_BIT      						0xA1
//
//#define RGBTime_init									0xC0
//#define WaitTime_init									0xF0
//#define ClearInterruptLowThresholdLowByte_init			0x00
//#define ClearInterruptLowThresholdHighByte_init			0x00
//#define ClearInterruptHighThresholdLowByte_init			0x00
//#define ClearInterruptHighThresholdHighByte_init		0x00
//#define InterruptPersistanceFilter_init					0x10
//#define Configuration_init								0x00
//#define GainControlRegister_init						0x00

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


void COL_ReadData(){

	uint8_t clear[2];
	uint8_t red[2];
	uint8_t green[2];
	uint8_t blue[2];

	RGB_SENSOR_WriteBlock(&ClearADClowDataRegister, 1, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_ReadBlock((uint8_t*)&clear, 2,RGB_SENSOR_SEND_STOP);

	WAIT_Waitms(1);

	RGB_SENSOR_WriteBlock(&RedADClowDataRegister, 1, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_ReadBlock((uint8_t*)&red, 2,RGB_SENSOR_SEND_STOP);

	WAIT_Waitms(1);

	RGB_SENSOR_WriteBlock(&GreenADClowDataRegister, 1, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_ReadBlock((uint8_t*)&green, 2,RGB_SENSOR_SEND_STOP);

	WAIT_Waitms(1);

	RGB_SENSOR_WriteBlock(&BlueADClowDataRegister, 1, RGB_SENSOR_DO_NOT_SEND_STOP);
	RGB_SENSOR_ReadBlock((uint8_t*)&blue, 2,RGB_SENSOR_SEND_STOP);

	WAIT_Waitms(1);

	uint16_t clearRes = 0;
	clearRes = (uint8_t)clear[1] << 8;
	clearRes |= clear[0];

	uint16_t redRes = 0;
	redRes = (uint8_t)red[1] << 8;
	redRes |= red[0];

	uint16_t greenRes = 0;
	greenRes = (uint8_t)green[1] << 8;
	greenRes |= green[0];

	uint16_t blueRes = 0;
	blueRes = (uint8_t)blue[1] << 8;
	blueRes |= blue[0];

//	SER_SendString("c: ");		// debugging
//	SER_SendUint16(clearRes);
//
//	SER_SendString("  r: ");
//	SER_SendUint16(redRes);
//
//	SER_SendString("  g: ");
//	SER_SendUint16(greenRes);
//
//	SER_SendString("  b: ");
//	SER_SendUint16(blueRes);
//	SER_SendNewLine();
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

