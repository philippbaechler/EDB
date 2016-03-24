#include "PE_Types.h"
#include "MOT_LEFT.h"
#include "MOT_RIGHT.h"

#include "MotionController.h"


MOT_FSMData motionController;

/*
 * Initializes the MotionController. Different values could be defined here. e.g. minimal period time
 * */
void MOT_Init(void){
	motionController.running = FALSE;
	motionController.state = MOT_FSM_STOP;
	motionController.max_common_period = 65000;
	motionController.min_common_period = 200; 		// 0.2 ms minimale Periodendauer -> grösst mogliche Geschwindigkeit
	motionController.accleration_value = 500;		// alle 10 ms wird die Periodendauer um 0.1 ms kleiner
}

/*
 * Here you can set the new maximal speed. It's called when the PI changes States or if we detect an container.
 * */
void MOT_SetMaxSpeed(int16_t master_speed_period){
	motionController.master_speed_period = master_speed_period;
}

/*
 * Here you can set the new speed. It's used when we acclerate or decclerate.
 * */
void MOT_SetSpeed(){

	MOT_CalcualteDifferential();

	MOT_LEFT_SetPeriodUS(motionController.motorLeft.actual_period);
	MOT_RIGHT_SetPeriodUS(motionController.motorRight.actual_period);
}

void MOT_ChangeState(MOT_StateKinds newState){
	motionController.state = newState;
}

/*
 * Preparation for the PID regulation. At the moment it's just a linear regulation.
 * Should just be called, when the differential-Value is changed.
 * */
void MOT_Regulate(){

}

/*
 * Calculates the differential between the two wheels and sets their individual periodtime (speed)
 * */
void MOT_CalcualteDifferential(){
	int8_t x = 1; // TODO: change the value x

	motionController.motorLeft.actual_period = motionController.actual_common_period + (x * motionController.differential);
	motionController.motorRight.actual_period = motionController.actual_common_period - (x * motionController.differential);
}

void MOT_Process(){

	switch(motionController.state){
		case MOT_FSM_STOP:
			motionController.actual_common_period = motionController.max_common_period;

			if(motionController.actual_common_period > motionController.target_common_period){
				motionController.state = MOT_FSM_ACCEL;
			}
			break;

		case MOT_FSM_ACCEL:

			motionController.actual_common_period = motionController.actual_common_period - motionController.accleration_value;

			if(motionController.actual_common_period <= motionController.target_common_period){
				motionController.state = MOT_FSM_RUN;
			}

			MOT_SetSpeed();
			break;

		case MOT_FSM_RUN:

			if(motionController.actual_common_period > motionController.target_common_period){
				motionController.state = MOT_FSM_ACCEL;
			}
			if(motionController.actual_common_period < motionController.target_common_period){
				motionController.state = MOT_FSM_DECEL;
			}

			break;

		case MOT_FSM_DECEL:

			motionController.actual_common_period = motionController.actual_common_period + motionController.accleration_value;

			if(motionController.actual_common_period > motionController.target_common_period){
				motionController.state = MOT_FSM_RUN;
			}
			if(motionController.actual_common_period == motionController.max_common_period){
				motionController.state = MOT_FSM_STOP;
			}

			MOT_SetSpeed();

			break;
	}
}
