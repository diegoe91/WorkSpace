/*============================================================================*/
/*                        SV C CE SOFTWARE GROUP                              */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*
* C Source:         dummy.c
* Instance:         RPL_1
* %version:         1
* %created_by:      Diego Flores
* %date_created:    Wend Jul  01 11:00:00 2015 %
*=============================================================================*/
/* DESCRIPTION : C source template file                                       */
/*============================================================================*/
/* FUNCTION COMMENT : This file describes the C source template according to  */
/* the new software platform                                                  */
/*                                                                            */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*  REVISION |   DATE      |                               |      AUTHOR      */
/*----------------------------------------------------------------------------*/
/*  1.0      | 07/01/2015  |                               | Diego Flores     */
/* Integration under Continuus CM                                             */
/*============================================================================*/

/* Includes */
/* -------- */
#include "typedefs.h"

/** Own headers */
#include "dummy.h"

/* GPIO routines prototypes */ 
#include "GPIO.h"

/* Functions macros, constants, types and datas         */
/* ---------------------------------------------------- */
/* Functions macros */

/*==================================================*/ 
/* Definition of constants                          */
/*==================================================*/ 
/* BYTE constants */


/* WORD constants */


/* LONG and STRUCTURE constants */

typedef enum
{
	IDLE,
	WINDOWMANUAL_OPENING,
	WINDOWMANUAL_CLOSING,
	WINDOWAUTO_OPENING,
	WINDOWAUTO_CLOSING,
	ANTI_PINCH
	
}STATES;

/*======================================================*/ 
/* Definition of RAM variables                          */
/*======================================================*/ 
/* BYTE RAM variables */

T_UBYTE rub_state = IDLE; 			/* Initialize the first state of the finite states machine */
T_SBYTE rsb_BarLed1=9;  			/* variable that controls the bar led */
T_UBYTE rub_auto_flag=0;			/* flag to control auto and manual mode */

/* WORD RAM variables */

T_UWORD ruw_time_counter = 0;  		/* Initialize the main time counter */
T_UWORD ruw_counter_anti_pinch =0; 	/* Initialize the anti pinch counter */
T_UWORD ruw_open =0; 			 	/* flag that checks if the window is totally open in the anti pinch */

/* LONG and STRUCTURE RAM variables */


/*======================================================*/ 
/* close variable declaration sections                  */
/*======================================================*/ 

/* Private defines */

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
#define ACTIVATED			  1
#define DEACTIVATED			  0

/* Private functions prototypes */
/* ---------------------------- */
void idle(void);
void windowmanual_opening(void);
void windowmanual_closing(void);
void windowauto_opening(void);
void windowauto_closing(void);
void anti_pinch(void);


/* Exported functions prototypes */
/* ----------------------------- */

/* Inline functions */
/* ---------------- */
/**************************************************************
 *  Name                 : inline_func	2
 *  Description          :
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :
 *  Critical/explanation :    [yes / No]
 **************************************************************/
 

/* Private functions */
/* ----------------- */
/**************************************************************
 *  Name                 : private_func
 *  Description          :
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :
 *  Critical/explanation :    [yes / No]
 **************************************************************/


/* Exported functions */
/* ------------------ */
/**************************************************************
 *  Name                 :	STATE_MACHINE
 *  Description          :  Finite State machine that controls all functions of the window lifter
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :  nothing
 *  Critical/explanation :    [yes / No]
 **************************************************************/
void STATE_MACHINE(void)
{
	switch (rub_state)
	{
		case IDLE:  
					idle();
					break;

		case WINDOWMANUAL_OPENING:
					windowmanual_opening();
					break;
		
		case WINDOWMANUAL_CLOSING:
					
					windowmanual_closing();
					break;
					
		case WINDOWAUTO_OPENING:
					windowauto_opening(); 
					break;
					
		case WINDOWAUTO_CLOSING:
					windowauto_closing();
					break;
					
		case ANTI_PINCH:
					anti_pinch();
					break;
					
		default: 	rub_state=	IDLE;
					ruw_time_counter=T_0ms;
					break;
	}
}

/**************************************************************
 *  Name                 :	idle
 *  Description          :  This function awaits for the correct detection of the buttons
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :  nothing
 *  Critical/explanation :    [yes / No]
 **************************************************************/

void idle(void)
{
	if(((WINDOW_OPEN == ACTIVATED) && (WINDOW_CLOSE == DEACTIVATED)) && (rsb_BarLed1 > BarLed_UnderFlow))
	{
		ruw_time_counter++;
		if(ruw_time_counter >= T_10ms)
		{
			rub_state=	WINDOWAUTO_OPENING;
			ruw_time_counter = T_0ms;		
		}
	}
	else if(((WINDOW_OPEN == DEACTIVATED) && (WINDOW_CLOSE == ACTIVATED)) && (rsb_BarLed1 < BarLed_OverFlow))
	{
		ruw_time_counter++;
		if(ruw_time_counter >= T_10ms)
		{
			rub_state=	WINDOWAUTO_CLOSING;
			ruw_time_counter = T_0ms;		
		}
	}
		else 
		{
			ruw_time_counter=T_0ms;
		}
}

/**************************************************************
 *  Name                 :	windowmanual_opening
 *  Description          :  function that opens the window in the manual mode
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :  nothing
 *  Critical/explanation :    [yes / No]
 **************************************************************/

void windowmanual_opening(void)
{
	if(((WINDOW_OPEN == ACTIVATED) && (WINDOW_CLOSE == DEACTIVATED)) && (rsb_BarLed1 > BarLed_UnderFlow))
					{
						if(ruw_time_counter>=T_800ms)
						{
							WINDOW_BARLED_OPEN(rsb_BarLed1);
					 		rsb_BarLed1--;
					 		ruw_time_counter=T_400ms;
						}
						ruw_time_counter++;
					}
					else
					{
						if(rsb_BarLed1 <= BarLed_UnderFlow)
						{
							rsb_BarLed1 =OPEN;
						}
						rub_state=	IDLE;
						ruw_time_counter=T_0ms;
						LED_OFF(LED_GREEN);
					}
}

/**************************************************************
 *  Name                 :	windowmanual_closing
 *  Description          :  function that closes the window in the manual mode
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :  nothing
 *  Critical/explanation :    [yes / No]
 **************************************************************/

void windowmanual_closing(void)
{
	if((ANTI_PINCH_ACTIVATED == ACTIVATED) && (rsb_BarLed1 < CLOSE))
					{
						ruw_counter_anti_pinch++;
						if(ruw_counter_anti_pinch >= BarLed_OverFlow)
						{
							rub_state=	ANTI_PINCH;
							ruw_time_counter=T_0ms;
							ruw_counter_anti_pinch=T_0ms;	
						}
					}
					else
					{
						ruw_counter_anti_pinch=T_0ms;
					}
					if(((WINDOW_OPEN == DEACTIVATED) && (WINDOW_CLOSE == ACTIVATED)) && (rsb_BarLed1 < BarLed_OverFlow))
					{
						if(ruw_time_counter>=T_800ms)
						{
							WINDOW_BARLED_CLOSE(rsb_BarLed1);
					 		rsb_BarLed1++;
					 		ruw_time_counter=T_400ms;
						}
						ruw_time_counter++;
					}
					else
					{
						if(rsb_BarLed1 >= BarLed_OverFlow)
						{
							rsb_BarLed1 =CLOSE;
						}
						rub_state=	IDLE;
						ruw_time_counter=T_0ms;
						LED_OFF(LED_BLUE);
					}
}

/**************************************************************
 *  Name                 :	windowauto_opening
 *  Description          :  function that opens the window in the auto mode
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :  nothing
 *  Critical/explanation :    [yes / No]
 **************************************************************/

void windowauto_opening(void)
{
	if(rsb_BarLed1>OPEN)
	{
		LED_ON(LED_GREEN);	
	}
		
					 if((((WINDOW_OPEN == ACTIVATED) && (WINDOW_CLOSE == DEACTIVATED)) && (rsb_BarLed1 > BarLed_UnderFlow) && rub_auto_flag == DEACTIVATED))
					 {
					 		if((ruw_time_counter==T_0ms || ruw_time_counter==T_400ms))
					 		{
					 			WINDOW_BARLED_OPEN(rsb_BarLed1);
					 			rsb_BarLed1--;	
					 		}
					 		else if(ruw_time_counter >= T_500ms)
					 		{
					 			rub_state=	WINDOWMANUAL_OPENING;
					 		}
					 }
					 else if(rsb_BarLed1 > BarLed_UnderFlow)
					 {	
					 	rub_auto_flag = ACTIVATED;
					 	if((ruw_time_counter==T_400ms) || (ruw_time_counter>=T_800ms))
					 	{
					 	    WINDOW_BARLED_OPEN(rsb_BarLed1);
					 		rsb_BarLed1--;
					 		if(ruw_time_counter>=T_800ms)
					 		{
					 			ruw_time_counter=T_400ms;
					 		}
					 	}
					 }
					 ruw_time_counter++;
					 if(rsb_BarLed1 <= BarLed_UnderFlow)
					 {	
					 	rub_auto_flag = DEACTIVATED;
					 	rub_state=	IDLE;
					 	ruw_time_counter=T_0ms;
					 	rsb_BarLed1=OPEN;
					 	LED_OFF(LED_GREEN);
					 }
}


/**************************************************************
 *  Name                 :	windowauto_closing
 *  Description          :  function that closes the window in the auto mode
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :  nothing
 *  Critical/explanation :    [yes / No]
 **************************************************************/
 
void windowauto_closing(void)
{
	if(rsb_BarLed1<CLOSE)
	{
		LED_ON(LED_BLUE);	
	}
					if((ANTI_PINCH_ACTIVATED == ACTIVATED) && (rsb_BarLed1 < CLOSE))
					{
						ruw_counter_anti_pinch++;
						if(ruw_counter_anti_pinch >= T_10ms)
						{
							rub_state=	ANTI_PINCH;
							ruw_time_counter=T_0ms;
							ruw_counter_anti_pinch=T_0ms;	
						}
					}
					else
					{
						ruw_counter_anti_pinch=T_0ms;
					}
					
					if((((WINDOW_OPEN == DEACTIVATED) && (WINDOW_CLOSE == ACTIVATED)) && (rsb_BarLed1 < BarLed_OverFlow)) && rub_auto_flag == DEACTIVATED)
					 {
					 		if((ruw_time_counter==T_10ms || ruw_time_counter==T_400ms))
					 		{
					 			WINDOW_BARLED_CLOSE(rsb_BarLed1);
					 			rsb_BarLed1++;	
					 		}
					 		else if(ruw_time_counter >= T_500ms)
					 		{
					 			rub_state=	WINDOWMANUAL_CLOSING;
					 		}
					 }
					 else if(rsb_BarLed1 < BarLed_OverFlow)
					 {
					 	rub_auto_flag = ACTIVATED;
					 	if((ruw_time_counter==T_400ms) || (ruw_time_counter>=T_800ms))
					 	{
					 	    WINDOW_BARLED_CLOSE(rsb_BarLed1);
					 		rsb_BarLed1++;
					 		if(ruw_time_counter>=T_800ms)
					 		{
					 			ruw_time_counter=T_400ms;
					 		}
					 	}
					 }
					 ruw_time_counter++;
					 if(rsb_BarLed1 >= BarLed_OverFlow)
					 {
					 	rub_auto_flag = DEACTIVATED;
					 	rub_state=	IDLE;
					 	rsb_BarLed1=CLOSE;
					 	ruw_time_counter=T_0ms;
					 	LED_OFF(LED_BLUE);
					 }
}

/**************************************************************
 *  Name                 :	anti_pinch
 *  Description          :  function that controls the anti pinch button
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :  nothing
 *  Critical/explanation :    [yes / No]
 **************************************************************/
 
void anti_pinch(void)
{
	if((rsb_BarLed1 > BarLed_UnderFlow) && (ruw_open == DEACTIVATED))
					 {
					 	LED_OFF(LED_BLUE);
					 	LED_ON(LED_GREEN);
					 	if(ruw_time_counter>=T_400ms)
					 	{
					 	    WINDOW_BARLED_OPEN(rsb_BarLed1);
					 		rsb_BarLed1--;
					 		if(ruw_time_counter>=T_400ms)
					 		{
					 			ruw_time_counter=T_0ms;
					 		}
					 	}
					 }
					 ruw_time_counter++;
					 if(rsb_BarLed1 == BarLed_UnderFlow)
					 {
					 	ruw_open = ACTIVATED;
					 	rsb_BarLed1=OPEN;
					 	LED_OFF(LED_GREEN);
					 }
					if(ruw_time_counter >= T_5s)
					{
						rub_state=	IDLE;
						ruw_time_counter=T_0ms;
						ruw_open = DEACTIVATED;
					}
}