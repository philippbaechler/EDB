#include "SurfaceScan.h"
#include "PE_Types.h"
#include "ADC.h"

bool SCN_Process(){
	bool isAContainer;

	uint16_t value;

	for(;;){

		ADC_MeasureChan(TRUE, 0);
		ADC_GetChanValue16(0, &value);

//		IR_SENSOR_Measure(TRUE);
//		IR_SENSOR_GetValue16(&value);
	}

	return isAContainer;
}
