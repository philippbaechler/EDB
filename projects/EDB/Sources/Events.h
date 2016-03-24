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
#include "CLS1.h"
#include "WAIT1.h"
#include "CS1.h"
#include "BT1.h"
#include "Serial1.h"
#include "ASerialLdd1.h"
#include "UTIL1.h"
#include "MOT_LEFT.h"
#include "PpgLdd1.h"
#include "SIG.h"
#include "MOT_RIGHT.h"
#include "PpgLdd2.h"

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

/*
** ===================================================================
**     Event       :  MOT_RIGHT_OnEnd (module Events)
**
**     Component   :  MOT_RIGHT [PPG]
**     Description :
**         This event is called when the specified number of iterations
**         is generated. (Only when the component is enabled - <Enable>
**         and the events are enabled - <EnableEvent>). The event is
**         available only when the peripheral supports an interrupt,
**         that is generated at the end of the PWM period.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MOT_RIGHT_OnEnd(void);

/*
** ===================================================================
**     Event       :  MOT_LEFT_OnEnd (module Events)
**
**     Component   :  MOT_LEFT [PPG]
**     Description :
**         This event is called when the specified number of iterations
**         is generated. (Only when the component is enabled - <Enable>
**         and the events are enabled - <EnableEvent>). The event is
**         available only when the peripheral supports an interrupt,
**         that is generated at the end of the PWM period.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MOT_LEFT_OnEnd(void);

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
