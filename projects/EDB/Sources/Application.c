#include "MotionController.h"
#include "WAIT.h"
#include "Ultrasonic.h"
#include "LED_RED.h"

static void APP_EventHandler(/*EVNT_Handle event*/){
}

void APP_Start(void){

	MOT_Init();
	US_Init();

	motionController.running = FALSE;
	motionController.target_common_period = motionController.max_common_period;

	for(;;){

		US_Measure();

		WAIT_Waitms(200);
	}
}
