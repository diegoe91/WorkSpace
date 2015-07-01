/*******************************************************************************/
/**
\file       dummy.c
\brief      Dummy Functions
\author     Francisco Martinez
\version    1.0
\date       04/04/2014
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Core modules */
/** Variable types and common definitions */
#include "typedefs.h"

/** Own headers */
#include "dummy.h"

/* GPIO routines prototypes */ 
#include "GPIO.h"

/** Used modules */

/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/

T_UBYTE rub_state = IDLE; 			/* Initialize the first state of the finite states machine */
T_UWORD ruw_time_counter = 0;  			/* Initialize the main time counter */
T_UWORD ruw_counter_anti_pinch =0; 	/* Initialize the anti pinch counter */
T_UWORD ruw_open =0; 			 	/* flag that checks if the window is totally open in the anti pinch */

T_SBYTE rsb_BarLed1=9;  /* variable that controls the bar led */
/*****************************************************************************************************
* Declaration of module wide FUNCTIONs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/


/****************************************************************************************************/
/**
* \brief    State machine that controls all functions of the window lifter 
* \author   Diego Flores
* \return   void
*/

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

/****************************************************************************************************/
/**
* \brief    This function awaits for the correct detection of the buttons  
* \author   Diego Flores
* \return   void
*/

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

/****************************************************************************************************/
/**
* \brief    function that opens the window in the mode manual 
* \author   Diego Flores
* \return   void
*/

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

/****************************************************************************************************/
/**
* \brief    function that closes the window in the mode manual  
* \author   Diego Flores
* \return   void
*/

void windowmanual_closing(void)
{
	if(ANTI_PINCH_ACTIVATED == ACTIVATED)
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

/****************************************************************************************************/
/**
* \brief    function that opens the window in the mode auto  
* \author   Diego Flores
* \return   void
*/


void windowauto_opening(void)
{
	if(rsb_BarLed1>OPEN)
	{
		LED_ON(LED_GREEN);	
	}
		
					 if(((WINDOW_OPEN == ACTIVATED) && (WINDOW_CLOSE == DEACTIVATED)) && (rsb_BarLed1 > BarLed_UnderFlow))
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
					 	rub_state=	IDLE;
					 	ruw_time_counter=T_0ms;
					 	rsb_BarLed1=OPEN;
					 	LED_OFF(LED_GREEN);
					 }
}

/****************************************************************************************************/
/**
* \brief    function that closes the window in the mode auto  
* \author   Diego Flores
* \return   void
*/


void windowauto_closing(void)
{
	if(rsb_BarLed1<CLOSE)
	{
		LED_ON(LED_BLUE);	
	}
					if(ANTI_PINCH_ACTIVATED == ACTIVATED)
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
					
					if(((WINDOW_OPEN == DEACTIVATED) && (WINDOW_CLOSE == ACTIVATED)) && (rsb_BarLed1 < BarLed_OverFlow))
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
					 	rub_state=	IDLE;
					 	rsb_BarLed1=CLOSE;
					 	ruw_time_counter=T_0ms;
					 	LED_OFF(LED_BLUE);
					 }
}

/****************************************************************************************************/
/**
* \brief    function that controls the anti pinch button  
* \author   Diego Flores
* \return   void
*/

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