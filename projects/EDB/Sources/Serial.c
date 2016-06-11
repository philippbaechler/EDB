#include "SERIAL_UART.h"
#include "WAIT.h"
#include "PE_Types.h"

#include "MotionController.h"
#include "ContainerRecognizer.h"
#include "Serial.h"
#include "RTOS.h"
#include "FRTOS1.h"

#include "Servos.h"
#include "SERVOS.h"

#include "PID.h"

#include "Bluetooth.h"

#include "Route_A_LED.h"
#include "Route_B_LED.h"

uint16_t SER_GetPeriod(uint8 c){

	uint16_t period;

	period = 1000000*0.1178/c;

	return period;
}

uint8_t SER_SerialProcess(){
	uint8_t c = 0;

	SERIAL_UART_ReadChar(&c);

	switch(c){
		case 1:		/* StopAfter */
			do{
				WAIT_Waitms(1); /* TODO: is this delay needed? */
				SERIAL_UART_ReadChar(&c);
			}while(c == 0);
			motionController.steps_left_until_stop = c/0.1178; // c in mm
			break;

		case 2:		/* EventAfterDistance */
			do{
				WAIT_Waitms(1); /* TODO: is this delay needed? */
				SERIAL_UART_ReadChar(&c);
			}while(c == 0);
			motionController.step_count = 0;
			motionController.step_count_target = (c*10)/0.1178; // c in cm
			break;

		case 3:		/* Run */
			do{
//				motionController.running = TRUE; // this could be implemented in the MOT_Task
				WAIT_Waitms(1); /* TODO: is this delay needed? */
				SERIAL_UART_ReadChar(&c);
			}while(c == 0);

			motionController.master_command_speed = c;
			motionController.target_common_period = SER_GetPeriod(motionController.master_command_speed);
			break;

		case 4:		/* SearchContainers */
			containerRecognizer.active = TRUE;
			break;

		case 5:		/* DoNotSearchContainers */
			containerRecognizer.active = FALSE;
			break;

		case 6:		/* Steer */
			do{
				WAIT_Waitms(1); /* TODO: is this delay needed? */
				SERIAL_UART_ReadChar(&c);
			}while(c == 0);

			PID_SetError(c);

			break;

		case 7:		/* SteerStraight */

			PID_SetError(0);

			break;

		case 8:		/* ROUTE_A */

			Route_A_LED_SetVal();
			Route_B_LED_ClrVal();

			break;

		case 9: 	/* ROUTE_B */

			Route_A_LED_ClrVal();
			Route_B_LED_SetVal();

			break;

		case 10:

			SRV_outlet();

			break;
	}
	return c;
}

void SER_SendEvent(){
	SERIAL_UART_SendChar('1');
}

void SER_SendString(unsigned char *msg){
	SERIAL_UART_SendStr(msg, SERIAL_UART_GetStdio()->stdOut);
}

void SER_SendUint16(uint16_t i){
	SERIAL_UART_SendNum16u(i,SERIAL_UART_GetStdio()->stdOut);
}

void SER_SendNewLine(){
	SERIAL_UART_SendChar('\r');
	SERIAL_UART_SendChar('\n');
}

void vSerialTask(){

	SER_SendString("\nEDB is ready!");

	for(;;){
		RTOS_Wait(3); 			// we only check every 3 ms the UART interfaces
		SER_SerialProcess();
	}
}

void SER_Init(){
	RTOS_AddTask(vSerialTask, "SER", 2);
}
