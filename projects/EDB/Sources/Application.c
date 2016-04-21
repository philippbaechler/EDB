#include "MotionController.h"
#include "WAIT.h"
#include "Ultrasonic.h"
#include "LED_RED.h"
#include "ContainerRecognizer.h"
#include "Serial.h"
#include "Bluetooth.h"
#include "RTOS.h"


static void APP_EventHandler(/*EVNT_Handle event*/){
}

void APP_Start(void){



	COR_Init();
	MOT_Init();
	SER_Init();
	BLT_Init();



	// Start RTOS here!
	RTOS_Init();

}
