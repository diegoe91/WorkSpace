/*============================================================================*/
/*                        SV C CE SOFTWARE GROUP                              */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*
* C Source:         %template.c%
* Instance:         RPL_1
* %version:         2 %
* %created_by:      uid02495 %
* %date_created:    Fri Jan  9 14:38:03 2004 %
*=============================================================================*/
/* DESCRIPTION : C source template file                                       */
/*============================================================================*/
/* FUNCTION COMMENT : General purpose IO functions                                                */
/*                                                                            */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*  REVISION |   DATE      |                               |      AUTHOR      */
/*----------------------------------------------------------------------------*/
/*  1.0      | DD/MM/YYYY  |                               | Mr. Template     */
/* Integration under Continuus CM                                             */
/*============================================================================*/

/* Includes */
/* -------- */

/** MCU derivative information */
#include "MCU_derivative.h"
/** Variable types and common definitions */
#include "typedefs.h"

/** Own headers */
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



/*======================================================*/ 
/* Definition of RAM variables                          */
/*======================================================*/ 
/* BYTE RAM variables */
T_SBYTE rsb_BarLed=0; 		/* variable that controls the bar led */

/* WORD RAM variables */


/* LONG and STRUCTURE RAM variables */


/*======================================================*/ 
/* close variable declaration sections                  */
/*======================================================*/ 

/* Private defines */
#define GPIO_INPUT					0
#define GPIO_OUTPUT					1
#define GPIO_OPEN_DRAIN_DISABLE		0
#define GPIO_OPEN_DRAIN_ENABLE		1

/* Private functions prototypes */
/* ---------------------------- */
void vfnGPIO_Output(uint8_t channel, uint8_t logical_value);
void vfnGPIO_FlashMainLED(void);


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
 *  Name                 : 	vfnGPIO_Output
 *  Description          : 	Drive the logical output value to the pin
 *  Parameters           :  uint8_t channel - GPIO channel to be configured
 							uint8_t logical_value - select the output value
 *  Return               :	void
 *  Critical/explanation :    [yes / No]
 **************************************************************/

void vfnGPIO_Output(uint8_t channel, uint8_t logical_value)
{
    SIU.GPDO[channel].B.PDO  = logical_value;  		/* Drive the logical output value to the pin */

}

/* Exported functions */
/* ------------------ */
/**************************************************************
 *  Name                 :	vfnGPIO_Init_channel
 *  Description          :  Configures individual GPIO pins to either input or output functionality.
 *  Parameters           :  uint8_t channel - GPIO channel to be configured
 							uint8_t input_output - selection of input/output functionality (has impact on output impedance selection)
 							uint8_t Open_drain - Push pull or open drain selection modes	
 *  Return               :	void
 *  Critical/explanation :    [yes / No]
 **************************************************************/

void vfnGPIO_Init_channel(uint8_t channel, uint8_t input_output, uint8_t Open_drain)
{
    if (input_output == GPIO_OUTPUT)
    {
    	SIU.PCR[channel].B.PA  = 0;  				/* GPIO */
    	SIU.PCR[channel].B.OBE = 1;					/* Output buffer enable */
    	if (Open_drain == GPIO_OPEN_DRAIN_ENABLE)
    	{
    		SIU.PCR[channel].B.ODE = 1;				/* Open drain option enable */	
    	}
    	else
    	{	
    		SIU.PCR[channel].B.ODE = 0;				/* Push-pull option enable */	
    	}
    }
    else if (input_output == GPIO_INPUT)
    {
    	SIU.PCR[channel].B.PA  = 0;  				/* GPIO */
    	SIU.PCR[channel].B.IBE = 1;					/* Input buffer enable */	
    }

}

/**************************************************************
 *  Name                 :	vfnGPIO_LED_Init
 *  Description          :  Initialize GPIO port connected to LEDs and buttons
 *  Parameters           :  [Input, Output, Input / output]
 *  Return               :	void
 *  Critical/explanation :    [yes / No]
 **************************************************************/

void vfnGPIO_LED_Init(void)
{
    /* Data Port E initialization */
	vfnGPIO_Init_channel(LED1,GPIO_OUTPUT,GPIO_OPEN_DRAIN_ENABLE);  /* PE4 --> LED1*/
	vfnGPIO_Output (LED1, 1);
	vfnGPIO_Init_channel(LED2,GPIO_OUTPUT,GPIO_OPEN_DRAIN_ENABLE);  /* PE5 --> LED2*/
	vfnGPIO_Output (LED2, 1);
	vfnGPIO_Init_channel(LED3,GPIO_OUTPUT,GPIO_OPEN_DRAIN_ENABLE);  /* PE6 --> LED3*/
	vfnGPIO_Output (LED3, 1);
	vfnGPIO_Init_channel(LED4,GPIO_OUTPUT,GPIO_OPEN_DRAIN_ENABLE);  /* PE7 --> LED4*/
	vfnGPIO_Output (LED4, 1);
	
	vfnGPIO_Init_channel(WINDOW_UP,GPIO_INPUT,GPIO_OPEN_DRAIN_DISABLE); /* PE0 --> Window up */
	vfnGPIO_Init_channel(WINDOW_DOWN,GPIO_INPUT,GPIO_OPEN_DRAIN_DISABLE); /* PE1 --> window down */
	vfnGPIO_Init_channel(ANTI_PINCH_BUTTON,GPIO_INPUT,GPIO_OPEN_DRAIN_DISABLE); /* PE3 --> anti pinch */
	
	/* Data Port A initialization */
	vfnGPIO_Init_channel(LED_BLUE,GPIO_OUTPUT,GPIO_OPEN_DRAIN_DISABLE);  /* PE6 --> LED5*/
	vfnGPIO_Output (LED_BLUE, 0);
	vfnGPIO_Init_channel(LED_GREEN,GPIO_OUTPUT,GPIO_OPEN_DRAIN_DISABLE);  /* PE7 --> LED6*/
	vfnGPIO_Output (LED_GREEN, 0);
	
    for(rsb_BarLed=0; rsb_BarLed<10; rsb_BarLed++)  /* PA0 - PA9 --> Bar led */
    {
    	vfnGPIO_Init_channel(rsb_BarLed,GPIO_OUTPUT,GPIO_OPEN_DRAIN_DISABLE);  
	    vfnGPIO_Output (rsb_BarLed, 1);
	 
    }
    rsb_BarLed=9;
}

/**************************************************************
 *  Name                 :	WINDOW_BARLED_OPEN
 *  Description          :  Turn off Barled's Leds 
 *  Parameters           :  T_SBYTE lsb_BarLed - It is the position of the window (Barled) 
 *  Return               :	void
 *  Critical/explanation :    [yes / No]
 **************************************************************/

void WINDOW_BARLED_OPEN (T_SBYTE lsb_BarLed) /* Turn off one led of the bar led */
{	
	LED_OFF(lsb_BarLed);
}

/**************************************************************
 *  Name                 :	WINDOW_BARLED_CLOSE
 *  Description          :  Turn on Barled's Leds 
 *  Parameters           :  T_SBYTE lsb_BarLed - It is the position of the window (Barled) 
 *  Return               :	void
 *  Critical/explanation :    [yes / No]
 **************************************************************/

void WINDOW_BARLED_CLOSE (T_SBYTE lsb_BarLed) /* Turn on one led of the bar led */
{
	LED_ON(lsb_BarLed);
}

/****************************************************************************************************/
