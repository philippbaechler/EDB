#include "SurfaceScan.h"
#include "PE_Types.h"
#include "ADC.h"
#include "MotionController.h"
#include "BLUETOOTH.h"
#include "UTIL1.h"
#include "WAIT.h"

uint32_t avgValue;


void SCN_CaptureRisingEdge(){

}

void SCN_MeasureLengthOfObject(){

}

bool SCN_IsAContainer(){
	bool isAContainer = FALSE;

	uint16_t IRvalue;

	uint8_t i;

	for(i = 0; i < 255; i++){

		WAIT_Waitus(1);

		ADC_MeasureChan(TRUE, 0);
		ADC_GetChanValue16(0, &IRvalue);
		avgValue = avgValue + IRvalue;
	}

	avgValue = avgValue / 255;

	return isAContainer;
}

static void SCN_PrintStatus(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendStatusStr((unsigned char*)"\r\nscn", (unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  IR value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(avgValue, io->stdOut);
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

void SCN_Init(){

}
