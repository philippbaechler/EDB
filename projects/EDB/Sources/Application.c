#include "MotionController.h"
#include "WAIT.h"
#include "Ultrasonic.h"
#include "LED_RED.h"
#include "ContainerRecognizer.h"
#include "Serial.h"

static void APP_EventHandler(/*EVNT_Handle event*/){
}

void APP_Start(void){

	MOT_Init();
	US_Init();
	COR_Init();

	motionController.running = TRUE;
	motionController.target_common_period = motionController.max_common_period;

	containerRecognizer.state = COR_FSM_RECOGNIZECOLOR;
	containerRecognizer.active = FALSE;

//	vContainerRecognizerTask();

	vSerialTask();

	// Start RTOS here!
}
