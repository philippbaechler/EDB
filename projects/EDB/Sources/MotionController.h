#define MOTORLEFT 1
#define MOTORRIGHT 2

#include "PE_Types.h" // it's needed to fix strange errors with data-types

typedef enum MOT_StateKinds {
	MOT_FSM_STOP,
	MOT_FSM_ACCEL,
	MOT_FSM_RUN,
	MOT_FSM_DECEL
} MOT_StateKinds;

typedef struct MOT_MOTOR {
	uint8_t index;
	bool dir;
	int16_t actual_period;

} MOT_MOTOR;

typedef struct MOT_FSMData {

	MOT_StateKinds state;
	bool running;

	uint16_t master_speed_period;	// Vorgabe von PI
	uint16_t min_common_period;		// entspricht der maximalen Geschwindigkeit
	uint16_t max_common_period;		// entspricht der minimalen Geschwindigkeit -> Stop Geschwindigkeit == 0
	uint16_t actual_common_period;	// momentane Geschwindigkeit
	uint16_t target_common_period;	// Zielgeschwidigkeit

	uint16_t accleration_value;		// Beschleunigungswert

	int8_t differential;			// Abweichung von der Mittellinie. -128 bis 127

	MOT_MOTOR motorLeft;
	MOT_MOTOR motorRight;

	uint16_t step_count; // uint16_t = 65536 -> reicht bei d = 60mm und 1/8 step mode f�r 7,7 Meter

} MOT_FSMData;


extern MOT_FSMData motionController;

void MOT_Init(void);
void MOT_SetMaxSpeed(int16_t max_speed);
void MOT_SetSpeed();
void MOT_ChangeState(MOT_StateKinds newState);
void MOT_Regulate();
void MOT_CalcualteDifferential();
void MOT_Process();
