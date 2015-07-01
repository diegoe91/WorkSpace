/*******************************************************************************/
/**
\file       dummy.h
\brief      Dummy Functions
\author     Francisco Martinez
\version    1.0
\date       04/04/2014
*/
/*******************************************************************************/

#ifndef _DUMMY_H        /*prevent duplicated includes*/
#define _DUMMY_H

/*-- Includes ----------------------------------------------------------------*/
#define T_0ms				  0
#define T_10ms	     		 10
#define T_400ms				400
#define T_500ms				500
#define T_800ms				800
#define T_5s			   5000
#define OPEN				  0
#define CLOSE				  9
#define BarLed_OverFlow		 10
#define BarLed_UnderFlow	 -1

/** Core modules */
/** Variable types and common definitions */

#include "typedefs.h"

/*-- Variables ---------------------------------------------------------------*/

/*-- Types Definitions -------------------------------------------------------*/

/* States of the finite state machine */
typedef enum
{
	IDLE,
	WINDOWMANUAL_OPENING,
	WINDOWMANUAL_CLOSING,
	WINDOWAUTO_OPENING,
	WINDOWAUTO_CLOSING,
	ANTI_PINCH
	
}STATES;

/*-- Defines -----------------------------------------------------------------*/


/*-- Function Prototypes -----------------------------------------------------*/

void STATE_MACHINE(void);
void idle(void);
void windowmanual_opening(void);
void windowmanual_closing(void);
void windowauto_opening(void);
void windowauto_closing(void);
void anti_pinch(void);

#endif /* _DUMMY_H */

/*******************************************************************************/
