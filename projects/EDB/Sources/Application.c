#include "MotionController.h"
#include "WAIT.h"
#include "LED_RED.h"
#include "ContainerRecognizer.h"
#include "Serial.h"
#include "BLUETOOTH.h"
#include "RTOS.h"
#include "Shell.h"
#include "PID.h"

static void APP_EventHandler(/*EVNT_Handle event*/){
}

void APP_Start(void){

	COR_Init();
	MOT_Init();
	SER_Init();
	SHELL_Init();
	WDG_Init();
	SRV_Init(); // Debug

	// Start RTOS here!
	RTOS_Init();

}
