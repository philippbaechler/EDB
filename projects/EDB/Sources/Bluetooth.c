#include "MotionController.h"


uint8_t c;

void BlueToothProcess(){
			//		CLS1_SendStr("HelloWorld\r\n", CLS1_GetStdio()->stdOut);
			//
			//		WAIT_Waitms(200);
/*
					BLUETOOTH_ReadChar(&c);

					if (c == 'h'){
						MOT_LEFT_Enable();
						motionController.target_common_period = 2500;
					}
					if (c == 'g'){
						MOT_LEFT_Enable();
						motionController.target_common_period = 32500;
					}
					if (c == 's'){
						motionController.target_common_period = 65000;
					}
					if (c == 'x'){
						motionController.differential = motionController.differential + 10;
						MOT_SetSpeed();
					}

					if (c == 'y'){
						motionController.differential = motionController.differential - 10;
						MOT_SetSpeed();
					}*/
}

void vBlueToothTask(){
	for(;;){
		BlueToothProcess();
	}
}
