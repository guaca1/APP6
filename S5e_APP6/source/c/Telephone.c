/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier principal Telephone.c
**  Auteurs : < LANF1601, DESK1902, SOUC2303, DUFD2703 >
**  Date : < 13/11/17 >
**
**  DIP 0 = Compounding
**  DIP 1 = Enregistrement
**  DIP 2 = Rejouer
**  DIP 3 = Changer protocole de communication
**  LED 0 = Compounding
**  LED 1 = Enregistrement en cours
**  LED 2 = RS-485
**  LED 3 = RS-232
**
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


char FLAG_LECTURE = false;
char FLAG_ECRITURE = false;
char FLAG_COMP = false;
char FLAG_RS232 = false;
char FLAG_RS485 = false;
char FLAG_ENRG = false;
char FLAG_REJOUE = false;

/****************************************************************************
	Extern content declaration :
****************************************************************************/

// déclaration des contenus utilisés ici mais définis ailleurs

extern far void vectors();   // Vecteurs d'interruption

short outputSample;

#pragma DATA_SECTION(enregistrement, ".EXT_RAM")
short enregistrement[SIZE_RECORD];
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
    short input;

	// initialisation des modules et des périphériques
    DSK6713_init();
    Audio_init();
	MCBSP_init();
	SPI_init();
	DSK6713_LED_init();
	DSK6713_DIP_init();

	//Initialisation des FLAGS selon les DIPS
	FLAG_RS232 = !DSK6713_DIP_get(3);
	FLAG_COMP = !DSK6713_DIP_get(0);
    if(FLAG_COMP)
    {
        DSK6713_LED_on(0);
    }
    else
    {
        DSK6713_LED_off(0);
    }


    // Boucle infinie
	while(1)
	{	
	    if(FLAG_ECRITURE)                                               //Si une valeur est disponible a lire
	    {
	        outputSample = ReadChar();
	        if ((outputSample & 1) == 1)                                //Si le dernier bit de la reception est 1, allumer la led 1 sinon eteindre
            {
                DSK6713_LED_on(1);
            }
            else
            {
                DSK6713_LED_off(1);
            }
	        if(FLAG_ENRG)                                               //Si un enregistrement est demande
	        {
	            enregistrement[compteurENRG++] = outputSample;          //remplir le tableau present dans la RAM
	            if(compteurENRG >= SIZE_RECORD)
	            {
	                compteurENRG = 0;
	                FLAG_ENRG = false;                                  //UNe fois le tableau plein, reset le FLAG
	            }
	        }

	        //write_AIC(temp);
	        FLAG_ECRITURE = false;                                      //Reset le FLAG
	    }
	    if (FLAG_LECTURE)                                               //Si une valeurest disponible a envoyer au micro
	    {
	        input = (short)input_sample();                              //prendre la valeur de l'AIC
	        PutChar(traiter_input((int)input));                         //traiter la donnee et envoyer en spi

	        FLAG_LECTURE = false;                                       //Reset flag
	    }
        if (!DSK6713_DIP_get(3) && FLAG_RS232)                          //LED 2  = RS-232
        {
            int temp = DSK6713_rget(DC_CNTL0);
            DSK6713_rset(DC_CNTL0, temp | 0x01);                        //set relay pour 232

            FLAG_RS232 = false;                                         //reset FLAG
            DSK6713_LED_on(3);
            DSK6713_LED_off(2);


        }
        else if (DSK6713_DIP_get(3) && !FLAG_RS232)                     //LED 3 = RS 485
        {
            int temp = DSK6713_rget(DC_CNTL0);
            DSK6713_rset(DC_CNTL0, temp & 0xFFFE);                      //set relay pour rs-485
            FLAG_RS232 = true;
            DSK6713_LED_on(2);
            DSK6713_LED_off(3);
        }

        if(!FLAG_ENRG && !FLAG_REJOUE)                                  // Si on demande un enregistrement et il n'est pas en mode rejouer ni deja en enregistrement
        {
            FLAG_ENRG = !DSK6713_DIP_get(1) ;                           //Dip 1 pour enregistrer
            if(!FLAG_ENRG)
            {
                FLAG_REJOUE = !DSK6713_DIP_get(2);                      // permettre de rejouer a la fin de l'enregistrement
            }
        }

        if (FLAG_COMP == DSK6713_DIP_get(0))                            //Mettre componding avec Dip 0
        {
            FLAG_COMP = !FLAG_COMP;
            if(FLAG_COMP)
            {
                DSK6713_LED_on(0);
            }
            else
            {
                DSK6713_LED_off(0);
            }
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
