
#include "WAIT.h"
#include "SIG.h"

#define grabberDutyCycleUS	100
#define	grabberLowerLimit		3000 // at 3V 	-> 1800 / 5V 	-> 3000
#define	grabberUpperLimit		6000 // 		-> 7500			-> 6000


void SRV_MoveServo(uint8_t degree, uint16_t dutyCycle, uint8_t servo){

	uint16_t dC;

	switch (servo){
		case 1:
			dC = TPM0_C2V;
			break;
		case 2:
			dC = TPM0_C3V;
			break;
		case 3:
			dC = TPM0_C4V;
			break;
		case 4:
			dC = TPM0_C5V;
			break;
	}
}

void SRV_retract(){

}

void SRV_release(){

	uint16_t i = TPM0_C4V;

	for (i; i >= grabberLowerLimit; i--){
		TPM0_C4V = i;
		WAIT_Waitus(grabberDutyCycleUS);
	}
}

void SRV_putBack(){

}

void SRV_loadOn(){

}

void SRV_grab(){

	uint16_t i = TPM0_C4V;

	for (i; i <= grabberUpperLimit; i++){
		TPM0_C4V = i;
		WAIT_Waitus(grabberDutyCycleUS);
	}
}

void SRV_extend(){

}

void SRV_pickUp(){

	SRV_extend();
	SRV_grab();
	SRV_grab();
	SRV_loadOn();
	SRV_putBack();
	SRV_release();
	SRV_retract();

}

void SRV_Init(){

	TPM0_C2V = 3000;
	TPM0_C3V = 3000;
	TPM0_C4V = grabberLowerLimit;
	TPM0_C5V = 3000;
}
