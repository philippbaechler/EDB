#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H

#define MOTORLEFT 1
#define MOTORRIGHT 2

#define DUTY	100		// T_on in us

#include "PE_Types.h"

typedef enum MOT_StateKinds {
	MOT_FSM_STOP,
	MOT_FSM_ACCEL,
	MOT_FSM_RUN,
	MOT_FSM_DECEL
} MOT_StateKinds;

typedef struct MOT_MOTOR {
	uint8_t index;
	bool dir;
	uint16_t actual_period;

} MOT_MOTOR;

typedef struct MOT_FSMData {

	MOT_StateKinds state;
	bool running;

	uint16_t master_speed_period;	// Vorgabe von PI
	uint16_t min_common_period;		// entspricht der maximalen Geschwindigkeit
	uint16_t max_common_period;		// entspricht der minimalen Geschwindigkeit -> Stop Geschwindigkeit : Periodendauer 65000
	uint16_t actual_common_period;	// momentane Geschwindigkeit
	uint16_t target_common_period;	// Zielgeschwidigkeit
	uint16_t accleration_counter;	// Variable um die Periodendauer für die Beschleunigung zu berechnen

	int8_t differential;			// Abweichung von der Mittellinie. -128 bis 127

	MOT_MOTOR motorLeft;
	MOT_MOTOR motorRight;

	uint16_t step_count; 			// uint16_t = 65536 -> reicht bei d = 60mm und 1/8 step mode für 7,7 Meter mit mm Auflösung
	uint16_t step_count_target;
	uint16_t steps_for_decleration;
	uint16_t steps_left_until_stop;
} MOT_FSMData;

extern MOT_FSMData motionController;

void MOT_Init(void);
void MOT_SetMaxSpeed(int16_t max_speed);
void MOT_SetSpeed();
void MOT_ChangeState(MOT_StateKinds newState);
void MOT_Regulate();
void MOT_SetPeriod();;
void MOT_CalcualteDifferential();
void MOT_CalcualteNOfSteps();
void MOT_StopAfterSteps(uint8_t nOfStepsLeft);
void MOT_Process();
void vMotionControlTask();

#endif /* MOTIONCONTROLLER_H */
