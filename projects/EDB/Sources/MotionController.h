#define MOTORLEFT 1
#define MOTORRIGHT 2

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

	int16_t min_common_period;		// entspricht der maximalen Geschwindigkeit
	int16_t actual_common_period;	// momentane Geschwindigkeit

	MOT_MOTOR motorLeft;
	MOT_MOTOR motorRight;

	uint16_t step_count; // reicht bei d = 60mm und 1/8 step mode für 7,7 Meter

} MOT_FSMData;


extern MOT_FSMData motorController;

void MOT_Init(void);
void MOT_SetSpeed(MOT_MOTOR* m_);
void MOT_Process(MOT_MOTOR* m_);
