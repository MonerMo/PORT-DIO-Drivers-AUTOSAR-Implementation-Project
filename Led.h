/******************************************************************************
 *
 * Module: Led
 *
 * File Name: Led.h
 *
 * Description: Header file for Led Module.
 *
 * Author: Mohamed Tarek
 ******************************************************************************/
#ifndef LED_H
#define LED_H

#include "Std_Types.h"
#include "Led_Cfg.h"

/* Description: 1. Fill the led configurations structure 
 *              2. Set the PIN direction which the led is connected as OUTPUT pin
 *		        3. Initialize the led ON/OFF according to the initial value
 */
void Led_Init(void);

/* Description: Set the LED state to ON */
void Led_SetOn(void);

/* Description: Set the LED state to OFF */
void Led_SetOff(void);

/*Description: Toggle the LED state */
void Led_Toggle(void);

/* Description: Refresh the LED state */
void Led_RefreshOutput(void);

#endif /* LED_H */
