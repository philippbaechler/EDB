#ifndef SERVOS_H
#define SERVOS_H

#include "PE_Types.h"
#include "BLUETOOTH.h"

#define grabberDutyCycleUS		200
#define	grabberLowerLimit		3200 // at 3V 	-> 1800 / 5V 	-> 3000
#define	grabberUpperLimit		5500 // 		-> 7500			-> 6000
#define SRV1park					4500
#define SRV2park					4500
#define SRV3park					4500

typedef struct SRV_Data {
										// Servo Value 0... 100
	uint16_t value1; 					// Stellwert Servo 1	--> Grosser Servor
	uint16_t value2; 					// Stellwert Servo 2	-->	Mittlerer Servo
	uint16_t value3; 					// Stellwert Servo 3	--> Greifarm
	uint16_t value4; 					// Stellwert Servo 4	--> Auslass

	uint16_t delay;						// sets the speed --> delay betwen a value inc- / decrementation

} SRV_Data;

extern SRV_Data servos;

void SRV_MoveServo(uint8_t degree, uint16_t dutyCycle, uint8_t servo);
void SRV_retract();
void SRV_release();
void SRV_putBack();
void SRV_loadOn();
void SRV_grab();
void SRV_extend(int extend_distance);
void SRV_pickUp();
void SRV_Init();
void SRV_setValue();

uint8_t SRV_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);

#endif /* SERVOS_H */
