#ifndef SERVOS_H
#define SERVOS_H

#include "PE_Types.h"
#include "BLUETOOTH.h"

//#define grabberDutyCycleUS		200

#define SRV1park					24			// TODO: Werte ermitteln
#define SRV2park					21

#define SRV1pos1					47
#define SRV2pos1					44

#define SRV1pos2					24
#define SRV2pos2					21

#define SRV1pos3					24
#define SRV2pos3					21

#define SRV3open					96
#define SRV3closed					57

#define SRV4open					96
#define SRV4closed					57

#define extendDistance				1			// Distanz um welche der Greifer sich pro Distanzmesszyklus dem Container nähert
#define SRV2extendLimit				50

typedef enum {
		slow	= 200,				// Wartezeit zwischen neuem Stellwert in ms
		medium	= 100,
		fast	= 50
	} speedModeType_t;

typedef struct SRV_Data {
									// Servo Value 0... 100
	float value1; 					// Stellwert Servo 1	--> Grosser Servor
	float value2; 					// Stellwert Servo 2	-->	Mittlerer Servo
	float value3; 					// Stellwert Servo 3	--> Greifarm
	float value4; 					// Stellwert Servo 4	--> Auslass

	speedModeType_t speed;				// sets the speed --> delay betwen a value inc- / decrementation
} SRV_Data;

extern SRV_Data servos;

void SRV_outlet();
void SRV_extend(int extend_distance);
void SRV_Init();
void SRV_setValue();
void SRV_moveArm(int srv1, int srv2, speedModeType_t speed);

uint8_t SRV_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);

#endif /* SERVOS_H */
