#ifndef CONTAINERRECOGNIZER_H
#define CONTAINERRECOGNIZER_H

#include "PE_Types.h"
#include "BLUETOOTH.h"

typedef enum COR_StateKinds {
	COR_FSM_OBSERVANT,
	COR_FSM_SURFACESCAN,
	COR_FSM_RECOGNIZECOLOR,
	COR_FSM_PICKUP
} COR_StateKinds;

typedef struct COR_FSMData {
	COR_StateKinds state;
	bool active;
} COR_FSMData;

extern COR_FSMData containerRecognizer;

void COR_Process();
void vContainerRecognizerTask(/*void* pvParameters*/);
uint8_t COR_ParseCommand(const uint8_t *cmd, bool *handled, BLUETOOTH_ConstStdIOType *io);
void COR_Init();

#endif /* CONTAINERRECOGNIZER_H */
