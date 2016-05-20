#include "Servos.h"
#include "WAIT.h"
#include "SIG.h"
#include "_6V_ON.h"
#include "stdlib.h"
#include "ColorSensor.h"

#include "SERIAL_UART.h"
#include "Serial.h"
#include "UTIL1.h"

#include "math.h"

SRV_Data servos;

void SRV_outlet(){
	servos.value4 = SRV4open;
	WAIT_Waitms(5000);
	servos.value4 = SRV4closed;
}

void SRV_extend(int extend_distance){
	if(extend_distance <= 20){
		SRV_moveArm(servos.value1 + extend_distance, servos.value2 + extend_distance, slow);
//		int i = 0;
//		while(i < extend_distance){
//			servos.value1 = servos.value1 + 1;
//			servos.value2 = servos.value2 + 1;
//			SRV_setValue();
//			WAIT_Waitms(100);						// Verzögerungszeit, sorgt für eine flüssige Bewegung ohne Schwingung
//			i = i + 1;
//		}
	}
}

void SRV_pickUp(){					//TODO: Rückgabewert boolean pickup erfolgreich J/N

	int SRV1posX, SRV2posX;

	if(servos.value1 == SRV1park && servos.value2 == SRV2park && servos.value3 == SRV3open){	// Wenn Servo in Parkier-position
		SRV_moveArm(SRV1pos1, SRV2pos1, fast);												// Greifarm schnell kurze Distanz ausfahren
		while(COL_ClearReachedPeak() == 0 && servos.value2 <= SRV2extendLimit){
			SRV_extend(extendDistance);													// Greifarm langsam unter stetigem Distanzmessen am Container annähern
		}
		SRV1posX = servos.value1;						//Position von Container merken
		SRV2posX = servos.value2;

		COL_ReadColors();
		WAIT_Waitms(1);

		if(COL_RightContainer() == 1){						// Containerfarbe prüfen
			servos.value3 = SRV3closed;					// Greifer schliessen
			SRV_moveArm(SRV1pos2,SRV2pos2,medium);		//medium
			WAIT_Waitms(1000);
			SRV_moveArm(SRV1pos3,SRV2pos3,slow);		//slow
			WAIT_Waitms(3000);
			SRV_moveArm(SRV1pos2,SRV2pos2,medium);		//medium
			SRV_moveArm(SRV1posX,SRV2posX,fast);
			servos.value3 = SRV3open;					// Greifer öffnen
		}
		SRV_moveArm(SRV1park,SRV2park,medium);			// Greifarm parkieren
		_6V_ON_ClrVal();								// Servo Speisung abschalten
	}
}

void SRV_moveArm(int srv1, int srv2, speedModeType_t speed){
	float srv1d = srv1 - servos.value1;				// Bewegungswinkel des Arms berechnen (Sollwert - Istwert)
	float srv2d = srv2 - servos.value2;

	float speed_f = speed;

	float stepValue1;
	float stepValue2;

	if(abs(srv1d) != 0 || abs(srv2d) != 0){				// min. ein Servo wert muss verändert sein (sonst division durch 0)
		if(abs(srv1d) > abs(srv2d)){					// Servo mit dem grösseren Bewegungswinkel auf |1| skalieren,
			stepValue1 = srv1d / abs(srv1d);					// anderer Servo mit einem Floating-Wert skalieren
			stepValue2 = srv2d / abs(srv1d);
		}
		else if(abs(srv1d) <= abs(srv2d)){											// Servo mit dem grösseren Bewegungswinkel auf |1| skalieren,
				stepValue2 = srv2d / abs(srv2d);				// anderer Servo mit einem Floating-Wert skalieren
				stepValue1 = srv1d / abs(srv2d);
		}

		while(servos.value1 != srv1 && servos.value2 != srv2){	// Solange Endwert nicht erreicht
			servos.value1 = servos.value1 + stepValue1;
			servos.value2 = servos.value2 + stepValue2;
			SRV_setValue();
			RTOS_Wait(speed_f);									// Konstante Wartezeit
		}
	}
	WAIT_Waitms(speed_f);
	servos.value1 = srv1;				// eventuelle Rundungsfehler von Floating Addition durch setzen der int Werte eliminieren
	servos.value2 = srv2;
	SRV_setValue();
}

void SRV_setValue(){
	TPM0_C2V = servos.value1*60 + 1500;
	TPM0_C3V = servos.value2*60 + 1500;
	TPM0_C4V = servos.value3*60 + 1500;
	TPM0_C5V = servos.value4*60 + 1500;
	_6V_ON_SetVal();
}

void SRV_Debug(){
	SRV_setValue();
}

void SRV_Init(){
	_6V_ON_ClrVal();
	servos.value1 = SRV1park;
	servos.value2 = SRV2park;
	servos.value3 = SRV3open;
	servos.value4 = SRV4closed;

	//SRV_moveArm(SRV1park, SRV2park, slow);

	//Following Code for Debuging purposes
	//------------------------------------
	//servos.value1 = SRV1p
//	SRV_setValue();
//
//	SRV_pickUp();
//
//	WAIT_Waitms(2000);
//
//	SRV_extend(20);
//
//	SRV_moveArm(SRV1pos2, SRV2pos2, fast);
//
//	WAIT_Waitms(2000);
//
//	SRV_moveArm(SRV1pos1, SRV2pos1, medium);
//	SRV_moveArm(SRV1park, SRV2park, slow);
//
//	WAIT_Waitms(2000);
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
	BLUETOOTH_SendHelpStr((unsigned char*)"  extend <value>", (unsigned char*)"simultaniously extends the arm by the distance of <value> \r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  move <srv1> <srv2> <speed>", (unsigned char*)"simultaniously moves the arm to the position of <srv1> and <srv2> by the speed of <speed> \r\n", io->stdOut);
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
	BLUETOOTH_SendNum32s(servos.speed, io->stdOut);
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
			servos.speed = val;
			SRV_Debug();
			*handled = TRUE;
		}
		else {
		       BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}
	} else if (UTIL1_strncmp((char*)cmd, (char*)"srv off", sizeof("srv off")-1) == 0) {
		_6V_ON_ClrVal();
		*handled = TRUE;
	} else if (UTIL1_strncmp((char*)cmd, (char*)"srv extend ", sizeof("srv extend ")-1) == 0) {
		p = cmd+sizeof("srv extend");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			SRV_extend(val);
			*handled = TRUE;
		}
		else {
	        BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}
	} else if (UTIL1_strncmp((char*)cmd, (char*)"srv move ", sizeof("srv move ")-1) == 0) {
		p = cmd+sizeof("srv move");
		int srv1,srv2;
		speedModeType_t speed;

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			srv1 = val;
			p = cmd+sizeof("srv move xx");
			if (UTIL1_xatoi(&p, &val)==ERR_OK){
				srv2 = val;
				p = cmd+sizeof("srv move xx xx");
				if (UTIL1_xatoi(&p, &val)==ERR_OK){
					SRV_moveArm(srv1,srv2,speed);
					*handled = TRUE;
				}
			}
		}
		else {
	        BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}
	}
	return res;
}
