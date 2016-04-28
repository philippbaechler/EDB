#include "PE_Types.h"
#include "MOT_LEFT.h"
#include "MOT_RIGHT.h"
#include "LED_RED.h"
#include "FRTOS1.h"

#include "SIG.h"


#include "MotionController.h"
#include "Serial.h"
#include "RTOS.h"

#include "Servos.h"

#include "WAIT.h"

#include "SERIAL_UART.h"

#include "MOT_LEFT_DIR.h"
#include "MOT_LEFT_M1.h"
#include "MOT_RGHT_NSLEEP.h"
#include "MOT_LEFT_M0.h"
#include "MOT_LEFT_NSLEEP.h"
#include "MOT_RIGHT_DIR.h"
#include "MOT_RIGHT_M0.h"
#include "MOT_RIGHT_M1.h"


MOT_FSMData motionController;

void MOT_AccelerateDeclerate(bool acclerate){
	motionController.actual_common_period = MOT_GetPeriod(acclerate);
}
void MOT_Steer(){

	motionController.motorLeft.actual_period = (100*motionController.actual_common_period)/(100 + motionController.steering_lock);	// may we have to change the direction
	motionController.motorRight.actual_period = (100*motionController.actual_common_period)/(100 - motionController.steering_lock);

}
void MOT_SetSpeed(){

	MOT_LEFT_SetPeriodUS(motionController.motorLeft.actual_period);
	MOT_LEFT_SetDutyUS(motionController.motorLeft.actual_period-DUTY);
	MOT_RIGHT_SetPeriodUS(motionController.motorRight.actual_period);
	MOT_RIGHT_SetDutyUS(motionController.motorRight.actual_period-DUTY);

	MOT_SetTicksUntilStop(); // if we change the speed, we always have to adjust the amount of steps we need for a deceleration
}

void MOT_SetTicksUntilStop(){
	motionController.steps_for_deceleration = (1000000/motionController.actual_common_period)*(1000000/motionController.actual_common_period)/(2*784);
}

uint16_t MOT_GetPeriod(bool acclerate){ // accelerate = 1 / decelerate = 0
	uint16_t newPeriod = 0xFF;

	if(acclerate){
		newPeriod = motionController.actual_common_period - (motionController.actual_common_period/(motionController.acceleration_counter + 2));
	}
	else{
		newPeriod = motionController.actual_common_period + (motionController.actual_common_period/(motionController.acceleration_counter + 2));
	}

	return newPeriod;
}

void MOT_CalcualteNOfSteps(){
	uint16_t newSteps = (10000 / motionController.actual_common_period); // works only when actual_common_period > 10000, we've got here an error in our distance measuring. Since we've normally have periods < 1000, this shoulden't be a problem!
	motionController.step_count = motionController.step_count + newSteps;

	if (motionController.step_count >= motionController.step_count_target && motionController.step_count_target != 0){
		SER_SendEvent(); /* sending an event if we reach the number of steps to the PI */
		motionController.step_count = 0;
		motionController.step_count_target = 0;
	}

	if (motionController.steps_left_until_stop > motionController.steps_for_deceleration/* && motionController.steps_left_until_stop != 0*/){
		motionController.steps_left_until_stop = motionController.steps_left_until_stop - newSteps;
	}
}

void MOT_Process(){

	switch(motionController.state){
		case MOT_FSM_STOP:

			LED_RED_Off();

			if(motionController.target_common_period < 0xffff){
				motionController.state = MOT_FSM_ACCEL;

				MOT_LEFT_Enable();
				MOT_RIGHT_Enable();
			}

			break;

		case MOT_FSM_ACCEL:

			MOT_CalcualteNOfSteps();

			motionController.acceleration_counter++;

			MOT_AccelerateDeclerate(TRUE);
			MOT_Steer();
			MOT_SetSpeed();

			/* check if we reached the target speed */
			if(motionController.actual_common_period <= motionController.target_common_period){
				motionController.state = MOT_FSM_RUN;
				LED_RED_On();
			}

			/* Check if we have to start the deceleration for a stop */
			if(motionController.steps_for_deceleration >= motionController.steps_left_until_stop && motionController.steps_left_until_stop != 0){
				motionController.target_common_period = 0xffff;
				motionController.state = MOT_FSM_DECEL;
			}

			break;

		case MOT_FSM_RUN:

			MOT_CalcualteNOfSteps();

			LED_RED_Off();

			MOT_Steer();
			MOT_SetSpeed(); // only for the steering

			if((motionController.actual_common_period-motionController.target_common_period) > (motionController.target_common_period/50)){
				motionController.state = MOT_FSM_ACCEL;
			}
			else if((motionController.target_common_period-motionController.actual_common_period) > (motionController.target_common_period/50)){
				motionController.state = MOT_FSM_DECEL;
			}


			/* Check if we have to start the deceleration for a stop */
			if(motionController.steps_for_deceleration >= motionController.steps_left_until_stop && motionController.steps_left_until_stop != 0){
				motionController.target_common_period = 0xffff;
				motionController.state = MOT_FSM_DECEL;
			}

			break;

		case MOT_FSM_DECEL:

			MOT_CalcualteNOfSteps();
			motionController.acceleration_counter--;

			MOT_AccelerateDeclerate(FALSE);
			MOT_Steer();
			MOT_SetSpeed();

			if(motionController.actual_common_period >= (motionController.target_common_period-25000)){

				if(motionController.acceleration_counter == 1){ // if the acceleration-counter is 1, we have to stop!
					motionController.running = FALSE;
					motionController.state = MOT_FSM_STOP;
					MOT_LEFT_Disable();
					MOT_RIGHT_Disable();
					motionController.steps_left_until_stop = 0;
				}
				else{
					motionController.state = MOT_FSM_RUN;
				}
				LED_RED_On();
			}

			break;
	}
}

void vMotionControlTask(){

	for(;;){
		// have to be executed exactly every 10 ms
		RTOS_Wait(10);

		if(motionController.running){
			MOT_Process();

			uint16_t i = motionController.actual_common_period;
		}
		else{
//			FRTOS1_taskYIELD();
		}
	}
}

/*
 * Initializes the MotionController. Different values could be defined here. e.g. minimal period time
 * */
void MOT_Init(void){

	MOT_LEFT_NSLEEP_ClrVal();
	MOT_LEFT_DIR_SetVal();
	MOT_LEFT_M0_SetVal();
	MOT_LEFT_M1_SetVal();

	MOT_RGHT_NSLEEP_SetVal();
	MOT_RIGHT_DIR_SetVal();
	MOT_RIGHT_M0_SetVal();
	MOT_RIGHT_M1_SetVal();

	SRV_Init(); // maybe we should initialize the servos when a button is pressed, like a command?

	motionController.running = FALSE;
	motionController.state = MOT_FSM_STOP;
	motionController.actual_common_period = 0xffff;
	motionController.target_common_period = 0xffff;
	motionController.steering_lock = 0; // drive straight

	MOT_LEFT_Disable();
	MOT_RIGHT_Disable();

	RTOS_AddTask(vMotionControlTask, "MOT", 3);
}
