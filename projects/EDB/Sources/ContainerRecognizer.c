#include "ContainerRecognizer.h"
#include "WAIT.h"
#include "Ultrasonic.h"
#include "SurfaceScan.h"
#include "ColorSensor.h"
#include "MotionController.h"
#include "RTOS.h"
#include "Serial.h"
#include "FRTOS1.h"
#include "SERIAL_UART.h"
#include "SIG.h"
#include "Servos.h"

#define DistanceIRSevos 100 // in mm
#define stepsAfterIR DistanceIRSevos / 0.1178

COR_FSMData containerRecognizer;

void COR_Process(){

	switch(containerRecognizer.state){
		case COR_FSM_STOP:

			containerRecognizer.state = COR_FSM_OBSERVANT; // is state STOP really needed?
			break;

		case COR_FSM_OBSERVANT:

			RTOS_Wait(100);
			US_Measure();
			break;

		case COR_FSM_SURFACESCAN:

			if (SCN_IsAContainer()){ // Continue here!
				motionController.steps_left_until_stop = stepsAfterIR; // n of Steps we have to go after we recognized a container
			}

			break;

		case COR_FSM_RECOGNIZECOLOR:

//			COL_ReadColors();

			if(motionController.state = MOT_FSM_STOP){ // wait until we stand still (at the right place)

				uint8_t i = 0;
				for (i; i < 5; i++){
					COL_ReadColors();
				}

				if(COL_RightContainer()){
					containerRecognizer.state = COR_FSM_PICKUP;
				}
				else{
					containerRecognizer.state = COR_FSM_STOP;
				}
			}
			break;
		case COR_FSM_PICKUP:
			if (TPM0_C4V == grabberLowerLimit){
				SRV_grab();
			}
			else if (TPM0_C4V == grabberUpperLimit){
				SRV_release();
			}
			break;
	}
}

void vContainerRecognizerTask(/*void* pvParameters*/){

	//COL_Init(); // Has to be implemented here, because we need interrupts for the i2c. And these interrupts are disabled because of the Setup from RTOS. They get enabled with the RTOS_startShedule!

	for(;;){
		if(containerRecognizer.active /*containerRecognizer.state != COR_FSM_STOP*/){ // maybe this task could set sleeping if its not used?
			COR_Process();
			RTOS_Wait(100);
		}
		else{
			FRTOS1_taskYIELD();
		}
	}
}

void COR_Init(){

	US_Init();

	containerRecognizer.active = FALSE;
	containerRecognizer.state = COR_FSM_STOP;

	RTOS_AddTask(vContainerRecognizerTask, "COR", 2);
}
