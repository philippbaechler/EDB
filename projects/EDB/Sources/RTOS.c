#include "PE_Types.h"
#include "Events.h"
//#include "Keys.h"
//#include "LED.h"
//#include "CommonApplication.h"
//#include "Shell.h"
#include "FRTOS.h"
#include "RTOS.h"

xQueueHandle RTOS_CreateQueue(void)
{
	return xQueueCreate(RTOS_QUEUE_LENGTH, RTOS_QUEUE_ITEM_SIZE);
}

void RTOS_EnqueueString(xQueueHandle handle, unsigned char* str)
{
	if(FRTOS1_xQueueIsQueueFullFromISR(handle) == pdFALSE)
	{
		char* buf;
		buf = pvPortMalloc(strlen((char*)str)+1);
		strcpy((char*)buf, (char*)str);
		xQueueSendToBack(handle, &buf, 0);
	}
}

char* RTOS_DequeueString(xQueueHandle handle)
{
	portBASE_TYPE result;
	char* str;
	char* buf;

	if(FRTOS1_uxQueueMessagesWaiting(handle))
	{
		result = xQueueReceive(handle, &str, 0);

		if(!result)
		{
			return NULL;
		}
		buf = pvPortMalloc(strlen(str)+1);
		strcpy(buf, str);
		vPortFree(str);
		return buf;
	}
	return NULL;
}

void RTOS_AddTask(void (*callback)(void*), signed char* taskName)
{
	if (FRTOS1_xTaskCreate(
			callback,						/* pointer to the task */
			taskName,						/* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE,		/* task stack size */
			(void*)NULL,					/* optional task startup argument*/
			tskIDLE_PRIORITY,				/* initial priority */
			(xTaskHandle*)NULL				/* optional task handle to create */
			) != pdPASS)
	{
		for(;;){}
	}
}

void RTOS_Wait(int ms)
{
	vTaskDelay(ms/portTICK_RATE_MS);
}

void RTOS_Init(void)
{
	FRTOS1_vTaskStartScheduler();
}





























