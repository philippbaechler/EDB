#include "SurfaceScan.h"
#include "PE_Types.h"
#include "IR_SENSOR.h"

bool Process(){
	bool isAContainer;

	uint16_t value;

	for(;;){
		IR_SENSOR_Measure(TRUE);
		IR_SENSOR_GetValue16(&value);
	}

	return isAContainer;
}
