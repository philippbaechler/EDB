#include "RGB_SENSOR.h"
#include "ColorSensor.h"
#include "Serial.h"
#include "LED_Enable_1.h"
#include "LED_Enable_2.h"
#include "BLUETOOTH.h"
#include "UTIL1.h"
#include "LED_Enable_1.h"
#include "LED_Enable_2.h"

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
uint8_t enable[] = {CommandCodeEnable, Enable_init};



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

void COL_TurnOnLED(){
	LED_Enable_1_SetVal();
	LED_Enable_2_SetVal();
}
void COL_TurnOffLED(){
	LED_Enable_1_ClrVal();
	LED_Enable_2_ClrVal();
}

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

//	colors.blueValue = 0;
//	colors.clearValue = 0;
//	colors.greenValue = 0;
//	colors.redValue = 0;

	colors.clearValue = COL_ReadClear();

	WAIT_Waitms(1);

	colors.redValue = COL_ReadRed();

	WAIT_Waitms(1);

	colors.greenValue = COL_ReadGreen();

	WAIT_Waitms(1);

	colors.blueValue = COL_ReadBlue();
}

bool COL_RightContainer(){
	bool rightContainer = FALSE;

	if (!(colors.redValue > colors.blueValue && colors.greenValue > colors.blueValue)){ // First implementation of the color-detection
		rightContainer = TRUE;
	} else if(colors.blueValue > colors.redValue && colors.greenValue > colors.redValue){
		rightContainer = TRUE;
	}

	return rightContainer;
}

/*
 * This is the function for the contactless positioning of the grabber. It's called by Servos.h
 * \return TRUE if a we reached the peak.
 * */
bool COL_ClearReachedPeak(){

	bool reachedPeak = FALSE;
	static uint16_t clearValues[ClearValuesArraySize];
	static uint8_t arrayCounter;
	static uint32_t averageValue;
	uint16_t newValue = 0;

	newValue = COL_ReadClear();

	if (newValue < (averageValue*0.9)){ // here, we decide if we had a peak
		reachedPeak = TRUE;
	}
		clearValues[arrayCounter] = newValue;

		// calculate average
		averageValue = 0;
		uint8_t i;
		for (i = 0; i <= (ClearValuesArraySize-1); i++){
			averageValue = averageValue + clearValues[i];
		}
		averageValue = averageValue/ClearValuesArraySize;

		// calculate position of the ring-buffer
		if (arrayCounter >= (ClearValuesArraySize-1)){
			arrayCounter = 0;
		} else{
			arrayCounter++;
		}


	return reachedPeak;
}

/*
 * Functions for the bluetooth interface
 * */
static void COL_PrintStatus(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendStatusStr((unsigned char*)"\r\ncol", (unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  clear value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(colors.clearValue, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  red value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(colors.redValue, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  green value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(colors.greenValue, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  blue value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(colors.blueValue, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
}
static void COL_PrintHelp(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendHelpStr((unsigned char*)"col", (unsigned char*)"Group of col commands\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows col help or status\r\n", io->stdOut);
}
uint8_t COL_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io){

	uint8_t res = ERR_OK;

	if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"col help")==0) {
		COL_PrintHelp(io);
		*handled = TRUE;
	} else if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"col status")==0) {
		COL_PrintStatus(io);
		*handled = TRUE;
	}

	return res;
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
	RGB_SENSOR_WriteBlock(enable, 2, RGB_SENSOR_SEND_STOP);
}

