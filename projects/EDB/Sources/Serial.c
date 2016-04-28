#include "SERIAL_UART.h"
#include "WAIT.h"
#include "PE_Types.h"

#include "MotionController.h"
#include "ContainerRecognizer.h"
#include "Serial.h"
#include "RTOS.h"
#include "FRTOS1.h"

#include "Servos.h"
#include "SIG.h"

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
				motionController.running = TRUE;
				WAIT_Waitms(1); /* TODO: is this delay needed? */
				SERIAL_UART_ReadChar(&c);
			}while(c == 0);

			motionController.target_common_period = SER_GetPeriod(c);
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

			motionController.steering_lock = c;

			break;

		case 7:		/* SteerStraight */
			motionController.steering_lock = 0;
			break;

		// below are just commands for debugging

		case 8:		/* grab or release the container */
			if (TPM0_C4V <= grabberLowerLimit+300){
				SRV_grab();
			}
			else{
				SRV_release();
			}
			break;

		case 9: 	/* change state of the ContainerRecognizer */
			switch (containerRecognizer.state){
				case COR_FSM_STOP:
					containerRecognizer.state = COR_FSM_OBSERVANT;
					SER_SendString("COR_FSM_OBSERVANT");
					SER_SendNewLine();
					break;
				case COR_FSM_OBSERVANT:
					containerRecognizer.state = COR_FSM_SURFACESCAN;
					SER_SendString("COR_FSM_SURFACESCAN");
					SER_SendNewLine();
					break;
				case COR_FSM_SURFACESCAN:
					containerRecognizer.state = COR_FSM_RECOGNIZECOLOR;
					SER_SendString("COR_FSM_RECOGNIZECOLOR");
					SER_SendNewLine();
					break;
				case COR_FSM_RECOGNIZECOLOR:
					containerRecognizer.state = COR_FSM_PICKUP;
					SER_SendString("COR_FSM_PICKUP");
					SER_SendNewLine();
					break;
				case COR_FSM_PICKUP:
					containerRecognizer.state = COR_FSM_STOP;
					SER_SendString("COR_FSM_STOP");
					SER_SendNewLine();
					break;
			}
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
		RTOS_Wait(1);
		if(SER_SerialProcess() == 0){
//			FRTOS1_taskYIELD();
		}
		else{
//			RTOS_Wait(1);
		}
//		WAIT_Waitms(1); // delete this?
	}
}

void SER_Init(){
	RTOS_AddTask(vSerialTask, "SER", 2);
}
