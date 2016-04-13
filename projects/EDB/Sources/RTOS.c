/**
 * \file
 * \brief Real Time Operating System (RTOS) main program.
 * \author Erich Styger, erich.styger@hslu.ch
 */

//#include "Platform.h"

#include "RTOS.h"
#include "FRTOS1.h"
//#include "Application.h"

void RTOS_AddTask(void (*callback)(void*), signed char* taskName, uint8_t priority) {
	if(FRTOS1_xTaskCreate(
			callback,  						/* pointer to the task */
	        taskName, 						/* task name for kernel awareness debugging */
	        configMINIMAL_STACK_SIZE, 		/* task stack size */
	        (void*)NULL,       				/* optional task startup argument */
			priority,  						/* initial priority */
	        (xTaskHandle*)NULL 				/* optional task handle to create */
	      ) != pdPASS) {
	    for(;;){}
	}
}

void RTOS_Wait(uint16_t ms) {
	vTaskDelay(ms/portTICK_RATE_MS);
}

void RTOS_Init(void) {
	FRTOS1_vTaskStartScheduler();
}

void RTOS_Deinit(void) {
  /* nothing needed for now */
}
