#include "PE_Types.h"
#include "MOT_LEFT.h"
#include "MOT_RIGHT.h"
#include "LED_RED.h"
#include "FRTOS1.h"
#include "UTIL1.h"

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


#include "Route_A_LED.h" // debugging
#include "_6V_ON.h"

MOT_FSMData motionController;

uint8_t counterPrescaler;

void MOT_Set_Kp(uint16_t p){
	motionController.Kp = p;
}
void MOT_Set_Ki(uint16_t i){
	motionController.Ki = i;
}
void MOT_Set_Kd(uint16_t d){
	motionController.Kd = d;
}
void MOT_CalculatePID(int8_t targetValue, int8_t actualValue){
	static int16_t esum = 0, e_old = 0;
	int16_t y_p, y_i, y_d, y;
	int16_t e;

	e = targetValue - actualValue;

	y_p = motionController.Kp * e;

	esum = esum + e;
	y_i = motionController.Ki * esum;

	y_d = motionController.Kd * (e_old - e);
	e_old = e;

	y = y_p + y_i + y_d;
//	int8_t y_res = (int8_t)(y/256);

	motionController.steering_lock = ((100 * y) / 32768)*(-1);
}

void MOT_AccelerateDeclerate(bool accelerate){
	motionController.actual_common_period = MOT_GetPeriod(accelerate);
}
void MOT_Steer(){

	if(counterPrescaler >= 5){

		_6V_ON_ClrVal();

		MOT_CalculatePID(0, motionController.error); // calculate the steeringLock here
		counterPrescaler = 0;

		_6V_ON_SetVal();

	}

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

			if(motionController.actual_common_period >= motionController.target_common_period && motionController.target_common_period != 0){

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

		if (motionController.target_common_speed < 0xffff){
			motionController.running = TRUE;
		}
		else
		{
			motionController.running = FALSE;
		}


		Route_A_LED_ClrVal();

		// have to be executed exactly every 10 ms

		FRTOS1_vTaskDelay(10);

		if(motionController.running){

			counterPrescaler++;

			MOT_Process();

			uint16_t i = motionController.actual_common_period;
		}
		else{
//			FRTOS1_taskYIELD();
		}

		Route_A_LED_SetVal();
	}
}

static void MOT_PrintStatus(const BLUETOOTH_StdIOType *io) {
  //TACHO_CalcSpeed(); /*! \todo only temporary until this is done periodically */
	BLUETOOTH_SendStatusStr((unsigned char*)"MOT", (unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  accl count", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(motionController.acceleration_counter, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  target common period", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(motionController.target_common_period, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
	BLUETOOTH_SendStatusStr((unsigned char*)"  actual common period", (unsigned char*)"", io->stdOut);
	BLUETOOTH_SendNum32s(motionController.actual_common_period, io->stdOut);
	BLUETOOTH_SendStr((unsigned char*)"\r\n", io->stdOut);
//  CLS1_SendStatusStr((unsigned char*)"  R speed", (unsigned char*)"", io->stdOut);
//  CLS1_SendNum32s(TACHO_GetSpeed(FALSE), io->stdOut);
//  CLS1_SendStr((unsigned char*)" steps/sec\r\n", io->stdOut);
}

static void MOT_PrintHelp(const BLUETOOTH_StdIOType *io) {
	BLUETOOTH_SendHelpStr((unsigned char*)"MOT", (unsigned char*)"Group of MOT commands\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows MOT help or status\r\n", io->stdOut);
	BLUETOOTH_SendHelpStr((unsigned char*)"  speed <value>", (unsigned char*)"Set speed\r\n", io->stdOut);
}

uint8_t MOT_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io){
	uint8_t res = ERR_OK;
	const unsigned char *p;
	int32_t val;

	if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"MOT help")==0) {
		MOT_PrintHelp(io);
		*handled = TRUE;
	} else if (UTIL1_strcmp((char*)cmd, (char*)BLUETOOTH_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"MOT status")==0) {
		MOT_PrintStatus(io);
		*handled = TRUE;
	} else if (UTIL1_strncmp((char*)cmd, (char*)"MOT speed ", sizeof("MOT speed ")-1) == 0) {
		p = cmd+sizeof("MOT speed");

		if (UTIL1_xatoi(&p, &val)==ERR_OK){
			motionController.target_common_period = SER_GetPeriod(val);
			*handled = TRUE;
		}
		else {
	        BLUETOOTH_SendStr((unsigned char*)"failed\r\n", io->stdErr);
		}

	}

	return res;
}

/*
 * Initializes the MotionController. Different values could be defined here. e.g. minimal period time
 * */
void MOT_Init(void){

	MOT_Set_Kp(1); // define the values for the PID-regulation
	MOT_Set_Ki(1);
	MOT_Set_Kd(0);

	MOT_LEFT_NSLEEP_ClrVal();
	MOT_LEFT_DIR_SetVal();
	MOT_LEFT_M0_SetVal();
	MOT_LEFT_M1_SetVal();
	MOT_LEFT_Disable();

	MOT_RGHT_NSLEEP_SetVal();
	MOT_RIGHT_DIR_SetVal();
	MOT_RIGHT_M0_SetVal();
	MOT_RIGHT_M1_SetVal();
	MOT_RIGHT_Disable();

	SRV_Init(); // maybe we should initialize the servos when a button is pressed, like a command?

	motionController.running = FALSE;
	motionController.state = MOT_FSM_STOP;
	motionController.actual_common_period = 0xffff;
	motionController.target_common_period = 0xffff;
	motionController.steering_lock = 0; // drive straight

	counterPrescaler = 0;

	RTOS_AddTask(vMotionControlTask, "MOT", 3);
}
