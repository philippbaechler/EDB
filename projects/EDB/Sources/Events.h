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
#include "BLUETOOTH.h"
#include "WAIT.h"
#include "CS1.h"
#include "BT1.h"
#include "Serial1.h"
#include "ASerialLdd1.h"
#include "UTIL1.h"
#include "MOT_LEFT.h"
#include "PpgLdd1.h"
#include "SERVOS.h"
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
#include "BitIoLdd10.h"
#include "BitIoLdd11.h"
#include "LED_Enable_2.h"
#include "BitIoLdd19.h"
#include "LED_GREEN.h"
#include "LEDpin2.h"
#include "ADC_IR_BATT.h"
#include "AdcLdd1.h"
#include "Shut_OFF.h"
#include "BitIoLdd13.h"
#include "Battery_low.h"
#include "BitIoLdd14.h"
#include "RGB_SENSOR.h"
#include "I2C1.h"
#include "KSDK1.h"
#include "FRTOS1.h"
#include "SYS1.h"
#include "RTT1.h"
#include "_6V_ON.h"
#include "BitIoLdd15.h"
#include "Route_B_LED.h"
#include "BitIoLdd16.h"
#include "Route_A_LED.h"
#include "BitIoLdd17.h"
#include "RGB_Sensor_LED.h"
#include "BitIoLdd18.h"
#include "LED_Enable_1.h"
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
**     Event       :  SERVOS_OnChannel0 (module Events)
**
**     Component   :  SERVOS [TimerUnit_LDD]
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
void SERVOS_OnChannel0(LDD_TUserData *UserDataPtr);

void ADC_IR_BATT_OnEnd(void);
/*
** ===================================================================
**     Event       :  ADC_IR_BATT_OnEnd (module Events)
**
**     Component   :  ADC_IR_BATT [ADC_IR_BATT]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void ADC_IR_BATT_OnCalibrationEnd(void);
/*
** ===================================================================
**     Event       :  ADC_IR_BATT_OnCalibrationEnd (module Events)
**
**     Component   :  ADC_IR_BATT [ADC_IR_BATT]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  SERVOS_OnChannel1 (module Events)
**
**     Component   :  SERVOS [TimerUnit_LDD]
*/
/*!
**     @brief
**         Called if compare register match the counter registers or
**         capture register has a new content. OnChannel1 event and
**         Timer unit must be enabled. See [SetEventMask] and
**         [GetEventMask] methods. This event is available only if a
**         [Interrupt] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void SERVOS_OnChannel1(LDD_TUserData *UserDataPtr);

void RGB_SENSOR_OnRequestBus(void);
/*
** ===================================================================
**     Event       :  RGB_SENSOR_OnRequestBus (module Events)
**
**     Component   :  RGB_SENSOR [GenericI2C]
**     Description :
**         User event which will be called before accessing the I2C bus.
**         Useful for starting a critical section.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RGB_SENSOR_OnReleaseBus(void);
/*
** ===================================================================
**     Event       :  RGB_SENSOR_OnReleaseBus (module Events)
**
**     Component   :  RGB_SENSOR [GenericI2C]
**     Description :
**         User event which will be called after accessing the I2C bus.
**         Useful for ending a critical section.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationStackOverflowHook(xTaskHandle pxTask, char *pcTaskName);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationStackOverflowHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         if enabled, this hook will be called in case of a stack
**         overflow.
**     Parameters  :
**         NAME            - DESCRIPTION
**         pxTask          - Task handle
**       * pcTaskName      - Pointer to task name
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationTickHook(void);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationTickHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         If enabled, this hook will be called by the RTOS for every
**         tick increment.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationIdleHook(void);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationIdleHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         If enabled, this hook will be called when the RTOS is idle.
**         This might be a good place to go into low power mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FRTOS1_vApplicationMallocFailedHook(void);
/*
** ===================================================================
**     Event       :  FRTOS1_vApplicationMallocFailedHook (module Events)
**
**     Component   :  FRTOS1 [FreeRTOS]
**     Description :
**         If enabled, the RTOS will call this hook in case memory
**         allocation failed.
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
