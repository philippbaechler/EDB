#include "WatchDog.h"
#include "RTOS.h"
#include "FreeRTOS.h"
#include "Shut_OFF.h"
#include "ADC.h"
#include "BLUETOOTH.h"
#include "UTIL1.h"
#include "Battery_low.h"

uint16_t voltage16_t;

/*
 * In this Function, we set a pin witch interrupts the power connection
 * */
void WDG_ShutOff(){

	Shut_OFF_SetVal();

}

/*
 * This Task runs with a frequency of 2 Hz and measures the battery voltage. If the voltage is too low, we decide to shut down.
 * */
void vWatchDogTask(){
	for(;;){
		ADC_MeasureChan(TRUE, 1);
		ADC_GetChanValue16(1, &voltage16_t);


		if (voltage16_t <= LowBatteryLight){
			if (voltage16_t <= LowBatteryBlink){

				Battery_low_NegVal(); // if the battery voltage is in a critical range, the LOW_BAT LED blinks with the frequency in witch this function runns

				if (voltage16_t <= ShutOffVoltage){
					WDG_ShutOff();
				}
				else{
					Shut_OFF_ClrVal();
				}
			}
			else{
				Battery_low_ClrVal();
				Battery_low_SetVal();
				Shut_OFF_ClrVal();
			}
		}
		else{
			Battery_low_ClrVal();
			Shut_OFF_ClrVal();
		}

		RTOS_Wait(500); // wait 500 ms between measurements
	}
}

char* WDG_GetVoltage(){
	char* voltage;

	char* r;
	r = (voltage16_t*11*33)/(0xffff*10*100);

	voltage = r[0]+r[1]+"."+r[2]+r[3];

	return voltage;
}

/*
 * Functions for the bluetooth interface
 * */
static void WDG_PrintStatus(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendStatusStr((unsigned char*)"\r\nwdg", (unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  battery voltage", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendStr(WDG_GetVoltage(), io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
}
static void WDG_PrintHelp(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendHelpStr((unsigned char*)"wdg", (unsigned char*)"Group of wdg commands\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows wdg help or status\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  shutdown", (unsigned char*)"Simulates a low battery! \r\n", io->stdOut);
}
uint8_t WDG_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io){
	uint8_t res = ERR_OK;

	if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"wdg help")==0) {
		WDG_PrintHelp(io);
		*handled = TRUE;
	} else if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"wdg status")==0) {
		WDG_PrintStatus(io);
		*handled = TRUE;
	} else if (UTIL1_strncmp((char*)cmd, (char*)"wdg shutdown ", sizeof("wdg shutdown ")-1) == 0){
		WDG_ShutOff();
		*handled = TRUE;
	}

	return res;
}

void WDG_Init(){
	RTOS_AddTask(vWatchDogTask, "WDG", 1);
}
