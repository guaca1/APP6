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
short CNTL0 = *(unsigned int *) DC_REG

bool FLAG_LECTURE = false;
bool FLAG_RS232 = true;
bool FLAG_RS485 = false;
//vos  #defines ou const int blablabla
//unique à ce fichier

/****************************************************************************
	Extern content declaration :
****************************************************************************/

// déclaration des contenus utilisés ici mais définis ailleurs

extern far void vectors();   // Vecteurs d'interruption


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
	myModule_init(); // initialisation du module exemple ; à enlever
	

	// Boucle infinie
	while(1)
	{	

        if (DSK6713_DIP_get(3) && !FLAG_RS232)                //Dip 3 ON = RS-232
        {
            CNTL0 = CNTL0 | 0x01;
            FLAG_RS232 = true;
            DSK6713_LED_on(2);
            DSK6713_LED_off(3);


        }
        else if (DSK6713_DIP_get(2) && !FLAG_RS485)           //Dip 2 ON = RS-485
        {
            CNTL0 = CNTL0 & 0xFFFE;
            FLAG_RS485 = true;
            DSK6713_LED_on(3);
            DSK6713_LED_off(2);
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
