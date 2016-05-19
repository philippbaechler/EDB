#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H

#define MOTORLEFT 1
#define MOTORRIGHT 2

#define DUTY	100		// T_on in us

#include "PE_Types.h"
#include "BLUETOOTH.h"

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

	MOT_MOTOR motorLeft;
	MOT_MOTOR motorRight;

	uint16_t master_command_speed;	// in ticks per second
	int8_t	steering_lock;			// in percent (-100% to +100%)

	uint16_t target_common_speed;	// in ticks per second
	uint16_t target_common_period;	// in us

	uint16_t actual_common_period;	// in us

	uint16_t acceleration_counter;	// we need this variable for remembering the state of acceleration

	uint16_t step_count; 				// uint16_t = 65536 -> reaches for d = 60mm and 1/8 step mode to 7,7 Meter in [mm] resolution
	uint16_t step_count_container;		// this one is used to measure the length of the container
	uint16_t step_count_target;			// for the event we have to send
	uint16_t steps_for_deceleration;
	uint16_t steps_left_until_stop;

//	uint16_t Kp, Ki, Kd;			// values for the pid regulation
//	int8_t error;


} MOT_FSMData;

extern MOT_FSMData motionController;

void MOT_Set_Kp(uint16_t);
void MOT_Set_Ki(uint16_t);
void MOT_Set_Kd(uint16_t);
void MOT_CalculatePID(int8_t targetValue, int8_t actualValue);

void MOT_SetMaxSpeed(int16_t max_speed);
void MOT_ChangeState(MOT_StateKinds newState);
void MOT_SetSpeed();
void MOT_SetTicksUntilStop();
uint16_t MOT_GetPeriod(bool accelerate);
void MOT_CalcualteNOfSteps();
void MOT_Process();
void vMotionControlTask();
void MOT_Init(void);

uint8_t MOT_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);

#endif /* MOTIONCONTROLLER_H */
