/* ###################################################################
**     Filename    : Events.h
**     Project     : EDB
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-03-21, 11:10, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "WAIT.h"
#include "CS1.h"
#include "UTIL1.h"
#include "MOT_LEFT.h"
#include "PpgLdd1.h"
#include "SIG.h"
#include "MOT_RIGHT.h"
#include "PpgLdd2.h"
#include "LED_RED.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "SERIAL_UART.h"
#include "MOT_LEFT_NSLEEP.h"
#include "BitIoLdd2.h"
#include "MOT_RGHT_NSLEEP.h"
#include "BitIoLdd3.h"
#include "MOT_LEFT_DIR.h"
#include "BitIoLdd4.h"
#include "MOT_RIGHT_DIR.h"
#include "BitIoLdd5.h"
#include "MOT_LEFT_M0.h"
#include "BitIoLdd6.h"
#include "MOT_LEFT_M1.h"
#include "BitIoLdd7.h"
#include "MOT_RIGHT_M0.h"
#include "BitIoLdd8.h"
#include "MOT_RIGHT_M1.h"
#include "BitIoLdd9.h"
#include "ULTRA_SONIC_TRIG.h"
#include "BitIoLdd10.h"
#include "COLOR_SENSOR_NLED.h"
#include "BitIoLdd11.h"
#include "LED_GREEN.h"
#include "LEDpin3.h"
#include "BitIoLdd13.h"
#include "IR_SENSOR.h"
#include "AdcLdd1.h"
#include "AS1.h"
#include "ASerialLdd2.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void);


/*
** ===================================================================
**     Event       :  SIG_OnChannel0 (module Events)
**
**     Component   :  SIG [TimerUnit_LDD]
*/
/*!
**     @brief
**         Called if compare register match the counter registers or
**         capture register has a new content. OnChannel0 event and
**         Timer unit must be enabled. See [SetEventMask] and
**         [GetEventMask] methods. This event is available only if a
**         [Interrupt] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void SIG_OnChannel0(LDD_TUserData *UserDataPtr);

void IR_SENSOR_OnEnd(void);
/*
** ===================================================================
**     Event       :  IR_SENSOR_OnEnd (module Events)
**
**     Component   :  IR_SENSOR [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void IR_SENSOR_OnCalibrationEnd(void);
/*
** ===================================================================
**     Event       :  IR_SENSOR_OnCalibrationEnd (module Events)
**
**     Component   :  IR_SENSOR [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
