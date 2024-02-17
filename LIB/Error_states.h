/*
 * Error_states.h
 *
 *  Created on: Feb 16, 2024
 *      Author: Nour Moharram
 */

#ifndef ERROR_STATES_H_
#define ERROR_STATES_H_


#define NULL_POINTER		(void *)0

typedef enum
{
	NOT_OK,
	OK,
	NULL_POINTER_ERROR,
	INVALID_INPUT_VALUE,
	GPIO_PIN_NUM_ERROR,
	GPIO_MODE_ERROR,
	GPIO_SPEED_ERROR,
	GPIO_PORT_ERROR,
	GPIO_STATUS_ERROR,
	LED_CONNECTION_MODE_ERROR,
	LED_DEFAULT_STATUS_ERROR,
}Sys_enuErrorStates_t;

#endif /* ERROR_STATES_H_ */
