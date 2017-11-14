/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier principal Telephone.c
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/ 

/***************************************************************************
	Include headers :
***************************************************************************/

// Used modules headers
#include "module_example.h"
#include "SPI_driver.h"
#include "Audio_driver.h"
#include "dsk6713.h"
#include "dsk6713_led.h"
#include "dsk6713_dip.h"
#include "C6713Helper_UdeS.h"


// standard libraries 
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition



/****************************************************************************
	Private macros and constants :
****************************************************************************/
#define USER_REG 0x90080000
#define DC_REG 0x90080008
#define MASK_GET_DIP3_BIT 0x00000080
#define MASK_GET_DIP2_BIT 0x00000040
#define CNTL0  *(unsigned int *) DC_REG;

MCBSP_Handle test_mcbsp;

char FLAG_LECTURE = false;
char FLAG_ECRITURE = false;
char FLAG_COMP = false;
char FLAG_RS232 = false;
char FLAG_RS485 = false;
char FLAG_dip1 = false;
char FLAG_ENRG = false;
char FLAG_REJOUE = false;
//vos  #defines ou const int blablabla
//unique à ce fichier

/****************************************************************************
	Extern content declaration :
****************************************************************************/

// déclaration des contenus utilisés ici mais définis ailleurs

extern far void vectors();   // Vecteurs d'interruption

short outputSample;

#pragma DATA_SECTION(enregistrement, ".EXT_RAM")
short enregistrement[160000];
int compteurENRG = 0;
/****************************************************************************
	Private Types :
****************************************************************************/

// type struct, enum , etc. definition here

// These type declaration are only valid in this .c

/****************************************************************************
	Private global variables :
****************************************************************************/

// Use static keyword here

/****************************************************************************
	Main program private functions prototypes :
****************************************************************************/

//déclarer vos prototype de fonction ici

/****************************************************************************
	Main Program :
****************************************************************************/

void main()
{
	// initialisation des modules et des périphériques
    DSK6713_init();
    Audio_init();
	myModule_init(); // initialisation du module exemple ; à enlever
	MCBSP_init();
	SPI_init();
	DSK6713_LED_init();
	DSK6713_DIP_init();
	short test;
	int temp;
	// Boucle infinie
	FLAG_RS232 = !DSK6713_DIP_get(3);
	FLAG_dip1 = DSK6713_DIP_get(1);
	FLAG_COMP != DSK6713_DIP_get(0);

	while(1)
	{	
	    if(FLAG_ECRITURE)
	    {
	        outputSample = ReadChar();
	        if ((outputSample & 1) == 1)
            {
                DSK6713_LED_on(1);
            }
            else
            {
                DSK6713_LED_off(1);
            }
	        if(FLAG_ENRG)
	        {
	            enregistrement[compteurENRG++] = outputSample;
	            if(compteurENRG >= 160000)
	            {
	                compteurENRG = 0;
	                FLAG_ENRG = false;
	            }
	        }

	        //write_AIC(temp);
	        FLAG_ECRITURE = false;
	    }
	    if (FLAG_LECTURE)
	    {
	        test = (short)input_sample();
	        PutChar(traiter_input((int)test));

	        FLAG_LECTURE = false;
	    }
        if (!DSK6713_DIP_get(3) && FLAG_RS232)                //LED 3  = RS-232
        {
            int temp = DSK6713_rget(DC_CNTL0);
            DSK6713_rset(DC_CNTL0, temp | 0x01);

            FLAG_RS232 = false;
            DSK6713_LED_on(2);
            DSK6713_LED_off(3);


        }
        else if (DSK6713_DIP_get(3) && !FLAG_RS232)       //LED 2 = RS 485
        {
            int temp = DSK6713_rget(DC_CNTL0);
            DSK6713_rset(DC_CNTL0, temp & 0xFFFE);
            FLAG_RS232 = true;
            DSK6713_LED_on(3);
            DSK6713_LED_off(2);
        }

        if(!FLAG_ENRG && !FLAG_REJOUE)
        {
            FLAG_ENRG = !DSK6713_DIP_get(1) ;                //ENLEVE POUR MERGE KC
            if(!FLAG_ENRG)
            {
                FLAG_REJOUE = !DSK6713_DIP_get(2);
            }
        }

        if (FLAG_COMP == DSK6713_DIP_get(0))
        {
            FLAG_COMP = !FLAG_COMP;
            DSK6713_LED_toggle(0);
        }

	}
}


/****************************************************************************
	Main program private functions :
****************************************************************************/


/****************************************************************************
	Main program interrupt service routines (ISR) :
****************************************************************************/

// end of Telephone.c
