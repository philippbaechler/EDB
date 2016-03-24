#include "MotionController.h"
#include "WAIT.h"

static void APP_EventHandler(/*EVNT_Handle event*/){
}

void APP_Start(void){

	MOT_Init();

	motionController.master_speed_period = 1000;

	for(;;){

		/*
		 * Unittest: MotionController
		 * starts with speed = 0. then acclerates to max speed, changes target speed, acclerates again and stops.
		 * */

		motionController.running = TRUE;

		motionController.target_common_period = motionController.master_speed_period;

		WAIT_Waitms(5000);

		motionController.target_common_period = 10000; // 10 ms Periodendauer

		WAIT_Waitms(5000);

		motionController.target_common_period = motionController.master_speed_period;

		WAIT_Waitms(5000);

		motionController.target_common_period = 60000; // 60 ms Periodendauer

		WAIT_Waitms(5000);
	}

}
