#include "Servos.h"
#include "WAIT.h"
#include "SIG.h"
#include "_6V_ON.h"

#include "SERIAL_UART.h"
#include "Serial.h"
#include "UTIL1.h"

SRV_Data servos;

void SRV_MoveServo(uint8_t degree, uint16_t dutyCycle, uint8_t servo){

	uint16_t dC = dutyCycle;

	switch (servo){
		case 1:
			dC = TPM0_C2V;
			break;
		case 2:
			dC = TPM0_C3V;
			break;
		case 3:
			dC = TPM0_C4V;
			break;
		case 4:
			dC = TPM0_C5V;
			break;
	}
}

void SRV_retract(){

}

void SRV_release(){

	uint16_t i = TPM0_C4V;

	for (i; i >= grabberLowerLimit; i--){
		TPM0_C4V = i;
		WAIT_Waitus(grabberDutyCycleUS);
	}
}

void SRV_putBack(){

}

void SRV_loadOn(){

}

void SRV_grab(){

	uint16_t i = TPM0_C4V;

	for (i; i <= grabberUpperLimit; i++){
		TPM0_C4V = i;
		WAIT_Waitus(grabberDutyCycleUS);
	}
}

void SRV_extend(){

}

void SRV_pickUp(){

	SRV_extend();
	SRV_grab();
	SRV_loadOn();
	SRV_putBack();
	SRV_release();
	SRV_retract();

}

void SRV_park(){

}

void SRV_Debug(){
//	for(int i = 0; i <= servo.value1; i++){
//
//		WAIT_Waitms(servos.delay);
//	}

//	while(servos.value1 != TM0_C2V & servos.value2 != TM0_C3V & servos.value3 != TM0_C3V & servos.value4 != TM0_C4V){
//
//	}

	TPM0_C2V = servos.value1*30 + 3000;
	TPM0_C3V = servos.value2*30 + 3000;
	TPM0_C4V = servos.value3*30 + 3000;
	TPM0_C5V = servos.value4*30 + 3000;

}

void SRV_Init(){

//	uint16_t i = grabberLowerLimit;
//
//	for(;;){
//	for (i; i >= grabberLowerLimit; i--){
//		TPM0_C4V = i;
//		WAIT_Waitus(grabberDutyCycleUS);
//	}
//
//	for (i; i <= grabberUpperLimit; i++){
//		TPM0_C4V = i;
//		WAIT_Waitus(grabberDutyCycleUS);
//	}
//}

	_6V_ON_SetVal();
	servos.value2 =
	TPM0_C2V = 4500;
	TPM0_C3V = 4500;
	TPM0_C4V = grabberLowerLimit;
	TPM0_C5V = 4500;
	//WAIT_Waitms(1500);
	//SRV_pickUp();
}


static void SRV_PrintHelp(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendHelpStr((unsigned char*)"srv", (unsigned char*)"Group of srv commands\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows srv help or status\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  1 <value>", (unsigned char*)"Set value (0... 100)\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  2 <value>", (unsigned char*)"Set value (0... 100)\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  3 <value>", (unsigned char*)"Set value (0... 100)\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  4 <value>", (unsigned char*)"Set value (0... 100)\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  delay <value>", (unsigned char*)"Set delay (0... 100ms)\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  off", (unsigned char*)"switches the srv power off \r\n", io->stdOut);
}

static void SRV_PrintStatus(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendStatusStr((unsigned char*)"\r\nsrv", (unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  value srv1", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(servos.value1, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  value srv2", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(servos.value2, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  value srv3", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(servos.value3, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  value srv4", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(servos.value4, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  value delay", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(servos.delay, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
}

uint8_t SRV_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io){
	uint8_t res = ERR_OK;
	const unsigned char *p;
	int32_t val;

	if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"srv help")==0) {
		SRV_PrintHelp(io);
		*handled = TRUE;
	} else if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"srv status")==0) {
		SRV_PrintStatus(io);
		*handled = TRUE;
	} else if (UTIL1_strncmp((char*)cmd, (char*)"srv 1 ", sizeof("srv 1 ")-1) == 0) {
		p = cmd+sizeof("srv 1");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			servos.value1 = val;
			SRV_Debug();
			*handled = TRUE;
		}
		else {
	        BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}
	} else if (UTIL1_strncmp((char*)cmd, (char*)"srv 2 ", sizeof("srv 2 ")-1) == 0) {
		p = cmd+sizeof("srv 2");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			servos.value2 = val;
			SRV_Debug();
			*handled = TRUE;
		}
		else {
		       BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}
	} else if (UTIL1_strncmp((char*)cmd, (char*)"srv 3 ", sizeof("srv 3 ")-1) == 0) {
		p = cmd+sizeof("srv 3");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			servos.value3 = val;
			SRV_Debug();
			*handled = TRUE;
		}
		else {
		       BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}
	} else if (UTIL1_strncmp((char*)cmd, (char*)"srv 4 ", sizeof("srv 4 ")-1) == 0) {
		p = cmd+sizeof("srv 4");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			servos.value4 = val;
			SRV_Debug();
			*handled = TRUE;
		}
		else {
		       BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}
	} else if (UTIL1_strncmp((char*)cmd, (char*)"srv delay ", sizeof("srv delay ")-1) == 0) {
		p = cmd+sizeof("srv delay");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			servos.delay = val;
			SRV_Debug();
			*handled = TRUE;
		}
		else {
		       BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}
	} else if (UTIL1_strncmp((char*)cmd, (char*)"srv off ", sizeof("srv off  ")-1) == 0) {
		_6V_ON_ClrVal();
		*handled = TRUE;
	}
	return res;
}
