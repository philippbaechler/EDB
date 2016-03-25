#include "MotionController.h"
#include "WAIT.h"

static void APP_EventHandler(/*EVNT_Handle event*/){
}

void APP_Start(void){

	MOT_Init();

	motionController.running = TRUE;
	motionController.target_common_period = motionController.max_common_period;

	for(;;){

		vSerialTask();
	}
}
