#include "SurfaceScan.h"
#include "PE_Types.h"
#include "ADC.h"

bool SCN_IsAContainer(){
	bool isAContainer = FALSE;

	uint16_t value;

	for(;;){

		ADC_MeasureChan(TRUE, 0);
		ADC_GetChanValue16(0, &value);
	}

	return isAContainer;
}
