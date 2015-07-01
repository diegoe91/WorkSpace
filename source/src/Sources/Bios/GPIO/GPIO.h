/*******************************************************************************/
/**

\file       GPIO.h
\brief      Macro definitions for registers access and I/O handling
\author     Abraham Tezmol
\version    0.2
\date       09/05/2013
*/
/*******************************************************************************/

#ifndef _GPIO_H        /*prevent duplicated includes*/
#define _GPIO_H

/*-- Includes ----------------------------------------------------------------*/

/** Core modules */
/** MCU derivative information */
#include "MCU_derivative.h"
/** Variable types and common definitions */
#include "typedefs.h"

/*-- Variables ---------------------------------------------------------------*/

/*-- Types Definitions -------------------------------------------------------*/





/*-- Defines -----------------------------------------------------------------*/

#define LED1      					68
#define LED2       					69
#define LED3       					70
#define LED4        				71

#define LED_BLUE					10
#define LED_GREEN					11
#define WINDOW_DOWN					65
#define WINDOW_UP					66
#define ANTI_PINCH_BUTTON			67
#define ACTIVATED					1
#define DEACTIVATED					0


#define GPIO_INPUT					0
#define GPIO_OUTPUT					1

#define GPIO_OPEN_DRAIN_DISABLE		0
#define GPIO_OPEN_DRAIN_ENABLE		1

/*-- Macros ------------------------------------------------------------------*/
/* Indicator LEDs handling */

/** Set LED */ 
#define LED_ON(channel)                     (SIU.GPDO[channel].B.PDO =  1)
/** Clear LED */ 
#define LED_OFF(channel)                    (SIU.GPDO[channel].B.PDO =  0)
/** Toggle LED */ 
#define LED_TOGGLE(channel)                 (SIU.GPDO[channel].B.PDO ^= 1) 
/** Reed Anti pinch button*/
#define ANTI_PINCH_ACTIVATED				(SIU.GPDI[ANTI_PINCH_BUTTON].B.PDI)
/** Reed window close button */
#define WINDOW_CLOSE						(SIU.GPDI[WINDOW_UP].B.PDI)
/** Reed window open button */
#define WINDOW_OPEN							(SIU.GPDI[WINDOW_DOWN].B.PDI)

/*-- Function Prototypes -----------------------------------------------------*/

void vfnGPIO_Init_channel(uint8_t channel, uint8_t input_output, uint8_t Open_drain);
void vfnGPIO_Output(uint8_t channel, uint8_t logical_value);
void vfnGPIO_FlashMainLED(void);
void vfnGPIO_LED_Init(void);
void WINDOW_BARLED_OPEN (T_SBYTE lsb_BarLed);
void WINDOW_BARLED_CLOSE (T_SBYTE lsb_BarLed);



#endif /* _GPIO_H */

/*******************************************************************************/
