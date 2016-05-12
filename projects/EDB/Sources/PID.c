#include "PID.h"
#include "BLUETOOTH.h"
#include "PE_Types.h"
#include "MotionController.h"
#include "UTIL1.h"

static PID_Config steering;

void PID_Set_Kp(uint16_t p){
	steering.Kp = p;
}
void PID_Set_Ki(uint16_t i){
	steering.Ki = i;
}
void PID_Set_Kd(uint16_t d){
	steering.Kd = d;
}
void PID_Set_W(uint16_t w){
	steering.W = w;
}
void PID_SetError(uint16_t error){
	steering.error = error;
}
void PID_CalculatePID(){
	int8_t targetValue = 0;
	int8_t actualValue = steering.error;

	static int16_t esum = 0, e_old = 0;
	int16_t y_p, y_i, y_d, y;
	int16_t e;

	e = targetValue - actualValue;

	y_p = steering.Kp * e;

	if (esum < steering.W){
		esum = esum + e;
		y_i = steering.Ki * esum;
	}

	y_d = steering.Kd * (e_old - e);
	e_old = e;

	y = y_p + y_i + y_d;
//	int8_t y_res = (int8_t)(y/256);

	motionController.steering_lock = ((100 * y) / 32768)*(-1);	// implementation for + 100% Version
//	motionController.steering_lock = ((10 * y) / 32768)*(-1);		// implementation for + 10% Version
//	motionController.steering_lock = ((y) / 32768)*(-1);		// implementation for + 1% Version
}

static void PID_PrintStatus(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendStatusStr((unsigned char*)"pid", (unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  p value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(steering.Kp, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  i value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(steering.Ki, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  d value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(steering.Kd, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  w value", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(steering.W, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  error", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(steering.error, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
}

static void PID_PrintHelp(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendHelpStr((unsigned char*)"pid", (unsigned char*)"Group of pid commands\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows PID help or status\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  p <value>", (unsigned char*)"Set p value\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  i <value>", (unsigned char*)"Set i value\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  d <value>", (unsigned char*)"Set d value\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  w <value>", (unsigned char*)"Set windup value\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  e <value>", (unsigned char*)"Set error for simulating PI\r\n", io->stdOut);
}

uint8_t PID_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io){
	uint8_t res = ERR_OK;
	const unsigned char *p;
	int32_t val;

	if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"pid help")==0) {
		PID_PrintHelp(io);
		*handled = TRUE;
	} else if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"pid status")==0) {
		PID_PrintStatus(io);
		*handled = TRUE;
	} else if (UTIL1_strncmp((char*)cmd, (char*)"pid p ", sizeof("pid p ")-1) == 0) {
		p = cmd+sizeof("pid p");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			steering.Kp = val;
			*handled = TRUE;
		}
		else {
	        BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}

	} else if (UTIL1_strncmp((char*)cmd, (char*)"pid i ", sizeof("pid i ")-1) == 0) {
		p = cmd+sizeof("pid i");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			steering.Ki = val;
			*handled = TRUE;
		}
		else {
	        BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}

	} else if (UTIL1_strncmp((char*)cmd, (char*)"pid d ", sizeof("pid d ")-1) == 0) {
		p = cmd+sizeof("pid d");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			steering.Kd = val;
			*handled = TRUE;
		}
		else {
	        BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}

	} else if (UTIL1_strncmp((char*)cmd, (char*)"pid w ", sizeof("pid w ")-1) == 0) {
		p = cmd+sizeof("pid w");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			steering.W = val;
			*handled = TRUE;
		}
		else {
	        BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}

	} else if (UTIL1_strncmp((char*)cmd, (char*)"pid e ", sizeof("pid e ")-1) == 0) {
		p = cmd+sizeof("pid e");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			steering.error = val;
			*handled = TRUE;
		}
		else {
	        BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}

	}

	return res;
}

void PID_Init(){

	PID_Set_Kp(10);
	PID_Set_Ki(1);
	PID_Set_Kd(0);
	PID_Set_W(1000);
	PID_SetError(0);
}
