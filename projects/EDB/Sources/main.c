/* ###################################################################
**     Filename    : main.c
**     Project     : EDB
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-03-21, 11:10, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "BLUETOOTH.h"
#include "WAIT.h"
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
#include "BitIoLdd11.h"
#include "LED_Enable_2.h"
#include "BitIoLdd19.h"
#include "LED_GREEN.h"
#include "LEDpin2.h"
#include "BitIoLdd12.h"
#include "ADC.h"
#include "AdcLdd1.h"
#include "Shut_OFF.h"
#include "BitIoLdd13.h"
#include "Battery_low.h"
#include "BitIoLdd14.h"
#include "RGB_SENSOR.h"
#include "I2C1.h"
#include "KSDK1.h"
#include "FRTOS1.h"
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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#include "Application.h"

#include "MotionController.h"

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  MOT_Set_Kp(10); // define the values for the PID-regulation
  MOT_Set_Ki(10);
  MOT_Set_Kd(0);

//  for (;;)
//  {
//	  MOT_CalculatePID(0,10);
//
//	  uint8_t s = motionController.steering_lock;
//  }

  APP_Start();

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
