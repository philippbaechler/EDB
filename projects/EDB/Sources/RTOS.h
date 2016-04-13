/**
 * \file
 * \brief RTOS interface.
 * \author Erich Styger, erich.styger@hslu.ch
 *
 * This module the main tasks of the RTOS.
 */

#include "PE_Types.h"

#ifndef RTOS_H_
#define RTOS_H_

void RTOS_AddTask(void (*)(void*), signed char*, uint8_t priority);

void RTOS_Wait(uint16_t ms);

/*! \brief Initializes the RTOS module */
void RTOS_Init(void);

/*! \brief De-Initializes the RTOS module */
void RTOS_Deinit(void);

#endif /* RTOS_H_ */
