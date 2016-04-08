#include "ContainerRecognizer.h"
#include "WAIT.h"
#include "Ultrasonic.h"
#include "SurfaceScan.h"
#include "ColorSensor.h"
#include "MotionController.h"

#define DistanceIRSevos 100 // in mm
#define stepsAfterIR DistanceIRSevos / 0.1178

COR_FSMData containerRecognizer;

void COR_Init(){
	containerRecognizer.active = FALSE;
	containerRecognizer.state = COR_FSM_STOP;
}

void COR_Process(){

	switch(containerRecognizer.state){
		case COR_FSM_STOP:
			containerRecognizer.state = COR_FSM_OBSERVANT;
			break;

		case COR_FSM_OBSERVANT:
			US_Measure();
			break;

		case COR_FSM_SURFACESCAN:
			if (SCN_IsAContainer()){ // Continue here!
				motionController.steps_left_until_stop = stepsAfterIR; // n of Steps we have to go after we recognized a container
			}
			break;

		case COR_FSM_RECOGNIZECOLOR:
			(void)COL_Process();
			break;

		case COR_FSM_PICKUP:
			break;
	}
}

void vContainerRecognizerTask(/*void* pvParameters*/){
	for(;;){
		if(containerRecognizer.active /*containerRecognizer.state != COR_FSM_STOP*/){ // maybe this task could set sleeping if its not used?
			COR_Process();
//			WAIT_Waitms(200);
		}
	}
}
