#include "SurfaceScan.h"
#include "PE_Types.h"
#include "ADC.h"
#include "MotionController.h"
#include "BLUETOOTH.h"
#include "UTIL1.h"
#include "WAIT.h"
#include "RTOS.h"

/*
 * Here we calculate a average value of the analog input. We need this one, because the signal has some peaks(200mV/1us) with a frequency (333 kHz).
 * \return analog input IR-sensor
 * */
uint32_t SCN_GetValue(){ // semaphores?

	uint32_t avgValue;
	uint16_t IRvalue;
	uint8_t i;

	// summing 200 measurements and wait between 1 us
	for(i = 0; i < 200; i++){

		WAIT_Waitus(1);

		ADC_MeasureChan(TRUE, 0);
		ADC_GetChanValue16(0, &IRvalue);
		avgValue = avgValue + IRvalue;

	}
	avgValue = avgValue / 200; // dividing by the number of measurements

	return avgValue;
}

/*
 * We wait here for a rising or falling edge.
 * */
void SCN_CaptureEdge(){

	uint32_t valuesBuf[10];
	uint32_t averageValue = 0;
	uint8_t arrayCounter;
	uint8_t i;

	bool edgeCaptured = FALSE;

	for (arrayCounter = 0; arrayCounter <= 9; arrayCounter++){ // fill the array with the first 10 values
		valuesBuf[arrayCounter] = SCN_GetValue();
		RTOS_Wait(waitBetweenMeasurementsMS);
	}

	arrayCounter = 0;

	do{
		valuesBuf[arrayCounter] = SCN_GetValue();
		RTOS_Wait(waitBetweenMeasurementsMS);

		/* calculate the average */
		for (i = 0; i <= 9; i++){
			averageValue = averageValue + valuesBuf[i];
		}
		averageValue = averageValue / 10;

		/* detect an edge */
		if (valuesBuf[arrayCounter] <= (percentFallingEdge * averageValue) || valuesBuf[arrayCounter] >= (percentRisingEdge * averageValue)){ // if the average value is changing more than +-20% we decide to see an edge
			edgeCaptured = TRUE;
		}
		else{

			if (arrayCounter >= 9){
			arrayCounter = 0;
			} else{
			arrayCounter++;
			}
		}
	} while(!edgeCaptured);

}

/*
 * Returns TRUE if we recognized a container. At the moment we do our decision only with the length of the Object.
 * */
bool SCN_IsAContainer(){
	bool isAContainer = FALSE;

	SCN_CaptureEdge();

	// reset step count
	motionController.step_count = 0;

	SCN_CaptureEdge();

	// compare step count
	if (motionController.step_count >= containerLengthMinSteps && motionController.step_count <= containerLengthMaxSteps){
		isAContainer = TRUE;
	}

	return isAContainer;
}


/*
 * Functions for the bluetooth interface
 * */
static void SCN_PrintStatus(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendStatusStr((unsigned char*)"\r\nscn", (unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  IR value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(SCN_GetValue(), io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
}
static void SCN_PrintHelp(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendHelpStr((unsigned char*)"scn", (unsigned char*)"Group of scn commands\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows scn help or status\r\n", io->stdOut);
}
uint8_t SCN_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io){

	uint8_t res = ERR_OK;

	if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"scn help")==0) {
		SCN_PrintHelp(io);
		*handled = TRUE;
	} else if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"scn status")==0) {
		SCN_PrintStatus(io);
		*handled = TRUE;
	}

	return res;
}

