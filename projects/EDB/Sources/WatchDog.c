#include "RTOS.h"
#include "FreeRTOS.h"
#include "Shut_OFF.h"
#include "ADC.h"
#include "BLUETOOTH.h"
#include "UTIL1.h"
#include "Battery_low.h"

#define LowBatteryLight 40116 // set these values corresponding to the battery-voltage-monitor
#define LowBatteryBlink 38924
#define ShutOffVoltage 37733

uint16_t voltage;

void WDG_ShutOff(){

	Shut_OFF_SetVal();

}

void vWatchDogTask(){
	for(;;){

		ADC_MeasureChan(TRUE, 1);
		ADC_GetChanValue16(1, &voltage);


		if (voltage <= LowBatteryLight){
			if (voltage <= LowBatteryBlink){

				Battery_low_NegVal(); // if the battery voltage is in a critical range, the LOW_BAT LED blinks with the frequency in witch this function runns

				if (voltage <= ShutOffVoltage){
					WDG_ShutOff();
				}
				else{
					Shut_OFF_ClrVal();
				}
			}
			else{
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

static void WDG_PrintStatus(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendStatusStr((unsigned char*)"\r\nwdg", (unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  battery voltage", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(voltage, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
}

static void WDG_PrintHelp(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendHelpStr((unsigned char*)"wdg", (unsigned char*)"Group of wdg commands\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows wdg help or status\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  shutdown", (unsigned char*)"Simulates a low battery! not yet! \r\n", io->stdOut);
}

uint8_t WDG_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io){
	uint8_t res = ERR_OK;
	const unsigned char *p;
	int32_t val;

	if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"wdg help")==0) {
		WDG_PrintHelp(io);
		*handled = TRUE;
	} else if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"wdg status")==0) {
		WDG_PrintStatus(io);
		*handled = TRUE;
	}

	return res;
}

void WDG_Init(){
	RTOS_AddTask(vWatchDogTask, "WDG", 1);
}
