#ifndef SERVOS_H
#define SERVOS_H

#include "PE_Types.h"

#define grabberDutyCycleUS		200
#define	grabberLowerLimit		3000 // at 3V 	-> 1800 / 5V 	-> 3000
#define	grabberUpperLimit		6000 // 		-> 7500			-> 6000

void SRV_MoveServo(uint8_t degree, uint16_t dutyCycle, uint8_t servo);
void SRV_retract();
void SRV_release();
void SRV_putBack();
void SRV_loadOn();
void SRV_grab();
void SRV_extend();
void SRV_pickUp();
void SRV_Init();

#endif /* SERVOS_H */
