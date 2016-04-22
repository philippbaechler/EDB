#include "SurfaceScan.h"
#include "PE_Types.h"
#include "ADC.h"
#include "Serial.h"
#include "MotionController.h"

uint16_t value;

void SCN_CaptureRisingEdge(){

}

void SCN_MeasureLengthOfObject(){

}

bool SCN_IsAContainer(){
	bool isAContainer = FALSE;

	ADC_MeasureChan(TRUE, 0);
	ADC_GetChanValue16(0, &value);

//	SER_SendUint16(value);
//	SER_SendNewLine();

//	for(;;){
//
//		ADC_MeasureChan(TRUE, 0);
//		ADC_GetChanValue16(0, &value);
//	}

	return isAContainer;
}
