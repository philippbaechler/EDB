#include "ULTRA_SONIC_TRIG.h"
#include "WAIT.h"
#include "SIG.h"
#include "LED_RED.h"
#include "ContainerRecognizer.h"

typedef enum {
  ECHO_IDLE, 		/* device not used */
  ECHO_TRIGGERED, 	/* started trigger pulse */
  ECHO_MEASURE, 	/* measuring echo pulse */
  ECHO_OVERFLOW, 	/* measurement took too long */
  ECHO_FINISHED 	/* measurement finished */
} US_EchoState;

typedef struct {
  LDD_TDeviceData *trigDevice; 	/* device handle for the Trigger pin */
  LDD_TDeviceData *echoDevice; 	/* input capture device handle (echo pin) */
  volatile US_EchoState state; 	/* state */
  SIG_TValueType capture; 	/* input capture value */
  uint16_t lastValue_us; 		/* last captured echo, in us */
} US_DeviceType;

static US_DeviceType usDevice; /* device handle for the ultrasonic device */
uint8_t range = 15;	/* defines the range(in cm) in which objects can be recognized */
uint16_t rangeInTics;

void US_Measure(){
	/* send 10 us pulse on TRIG line. */
	ULTRA_SONIC_TRIG_SetVal();
	WAIT_Waitus(10);
	usDevice.state = ECHO_TRIGGERED;
	ULTRA_SONIC_TRIG_ClrVal();
}

void US_EventEchoCapture(){
	if (usDevice.state == ECHO_TRIGGERED){
		SIG_ResetCounter(usDevice.echoDevice); // TODO: are there any problems with the other devices?
		usDevice.state = ECHO_MEASURE;
	}
	else if(usDevice.state == ECHO_MEASURE){
		SIG_GetCaptureValue(usDevice.echoDevice, 1, &usDevice.capture);
		usDevice.state = ECHO_FINISHED;

		if(usDevice.capture <= rangeInTics){
			// Change state to "Object captured"!
//			LED_RED_On();
			containerRecognizer.state = COR_FSM_SURFACESCAN;
		}
		else{
			// Stay in state "listening"!
//			LED_RED_Off();

			/* just doing nothing here */
		}
	}
}

void US_Init(){
	rangeInTics = (((range * 10000) / 1047) * 2); // TODO: Check this! (1047?)
}

//void vUltrasonicTask(void* pvParameters) {
//	for (;;) {
//		(void)US_Measure_us();
//		WAIT_Waitms(200);//RTOS_Wait(10);
//	}
//}

