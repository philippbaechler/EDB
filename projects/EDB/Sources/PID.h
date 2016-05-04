#include "PE_Types.h"
#include "BLUETOOTH.h"

typedef struct{
	uint16_t Kp, Ki, Kd;			// values for the pid regulation
	int8_t error;					// -128 to 127
} PID_Config;

void PID_CalculatePID();
void PID_SetError(uint16_t error);
uint8_t PID_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);
void PID_Init();
