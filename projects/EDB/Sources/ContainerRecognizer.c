#include "ContainerRecognizer.h"
#include "WAIT.h"
#include "Ultrasonic.h"
#include "SurfaceScan.h"
#include "ColorSensor.h"
#include "MotionController.h"
#include "RTOS.h"
#include "Serial.h"
#include "FRTOS1.h"
#include "SERIAL_UART.h"
#include "SIG.h"
#include "Servos.h"
#include "BLUETOOTH.h"
#include "LED_Enable_1.h"
#include "LED_Enable_2.h"

#define DistanceIRSevos 30 // in mm
#define stepsAfterIR DistanceIRSevos / 0.1178

COR_FSMData containerRecognizer;

void COR_TurnOnLED(){
	LED_Enable_1_SetVal();
	LED_Enable_2_SetVal();
}
void COR_TurnOffLED(){
	LED_Enable_1_ClrVal();
	LED_Enable_2_ClrVal();
}

void COR_Process(){

	switch(containerRecognizer.state){

		case COR_FSM_OBSERVANT:
			US_Measure();
			break;

		case COR_FSM_SURFACESCAN:
			if (SCN_IsAContainer()){
				motionController.steps_left_until_stop = stepsAfterIR; // n of Steps we have to go after we recognized a container
				containerRecognizer.state = COR_FSM_RECOGNIZECOLOR;
			}
			break;

		case COR_FSM_RECOGNIZECOLOR:
			if(motionController.state == MOT_FSM_STOP){ // wait until we stand still (at the right place)

				COR_TurnOnLED();

				RTOS_Wait(1000);

				uint8_t i = 0;
				for (i; i < 5; i++){ // Measure several times the color values to get a better value
					COL_ReadColors();
				}

				if(COL_RightContainer()){
					containerRecognizer.state = COR_FSM_RECOGNIZECOLOR;//COR_FSM_PICKUP;
				}
				else{
					containerRecognizer.state = COR_FSM_RECOGNIZECOLOR;//COR_FSM_OBSERVANT;
				}

				RTOS_Wait(1000);

				COR_TurnOffLED();
			}
			break;

		case COR_FSM_PICKUP:

			SRV_pickUp();

			break;
	}
}

void vContainerRecognizerTask(/*void* pvParameters*/){

	// cannot reach the color-sensor
	// We only can initialize the color-sensor, if it is connected. Otherwise, we get errors and cannot debug.
	COL_Init(); // Has to be implemented here, because we needd interrupts for the i2c. And these interrupts are disabled because of the Setup from RTOS. They get enabled with the RTOS_startShedule!

	for(;;){

		RTOS_Wait(100);

		if(containerRecognizer.active){
			COR_Process();
		}
		else{
//			FRTOS1_taskYIELD();
		}
	}
}

/*
 * Functions for the bluetooth interface
 * */
static void COR_PrintStatus(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendStatusStr((unsigned char*)"\r\ncor", (unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  active", (unsigned char*)"", io->stdOut);

	if (containerRecognizer.active){
		BLUETOOTH_SendStr((unsigned char*)"TRUE", io->stdOut);
	} else{
		BLUETOOTH_SendStr((unsigned char*)"FALSE", io->stdOut);
	}
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);

	BLUETOOTH_SendStatusStr((unsigned char*)"  state", (unsigned char*)"", io->stdOut);
	switch (containerRecognizer.state) {
		case COR_FSM_OBSERVANT:
			BLUETOOTH_SendStr((unsigned char*)"OBSERVANT", io->stdOut);
			break;
		case COR_FSM_SURFACESCAN:
			BLUETOOTH_SendStr((unsigned char*)"SURFACESCAN", io->stdOut);
			break;
		case COR_FSM_RECOGNIZECOLOR:
			BLUETOOTH_SendStr((unsigned char*)"RECOGNIZECOLOR", io->stdOut);
			break;
		case COR_FSM_PICKUP:
			BLUETOOTH_SendStr((unsigned char*)"PICKUP", io->stdOut);
			break;
	}
}
static void COR_PrintHelp(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendHelpStr((unsigned char*)"cor", (unsigned char*)"Group of cor commands\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows cor help or status\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  state <value>", (unsigned char*)"Sets the state\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  active <0/1>", (unsigned char*)"active TRUE or FALSE\r\n", io->stdOut);
}
uint8_t COR_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io){

	uint8_t res = ERR_OK;
	const unsigned char *p;
	int32_t val;

	if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"cor help")==0) {
		COR_PrintHelp(io);
		*handled = TRUE;
	} else if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"cor status")==0) {
		COR_PrintStatus(io);
		*handled = TRUE;
	} else if (UTIL1_strncmp((char*)cmd, (char*)"cor state ", sizeof("cor state ")-1) == 0) {
		p = cmd+sizeof("cor state");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){

			switch (val) {
				case 0:
					containerRecognizer.state = COR_FSM_OBSERVANT;
					break;
				case 1:
					containerRecognizer.state = COR_FSM_SURFACESCAN;
					break;
				case 2:
					containerRecognizer.state = COR_FSM_RECOGNIZECOLOR;
					break;
				case 3:
					containerRecognizer.state = COR_FSM_PICKUP;
					break;
				default:
					BLUETOOTH_SendStr((unsigned char*)"State not found!\r\n", io->stdOut);
					break;
			}

			*handled = TRUE;
		}
		else {
		       BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}

//		*handled = TRUE;
	} else if (UTIL1_strncmp((char*)cmd, (char*)"cor active ", sizeof("cor active ")-1) == 0){
		p = cmd+sizeof("cor active");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			if (val){
				containerRecognizer.active = TRUE;
			} else{
				containerRecognizer.active = FALSE;
			}

			*handled = TRUE;
		} else{
			BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}
	}

	return res;
}

void COR_Init(){

	US_Init();

	containerRecognizer.active = FALSE;
	containerRecognizer.state = COR_FSM_SURFACESCAN;

	RTOS_AddTask(vContainerRecognizerTask, "COR", 2);
}
