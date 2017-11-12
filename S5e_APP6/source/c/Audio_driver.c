/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier Audio_driver.c
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/

/***************************************************************************
	Include headers :
***************************************************************************/

//#include "something.h"


/***************************************************************************
	Include Module Header :
***************************************************************************/

#define AUDIO_DRIVER_MODULE_IMPORT
#include "Audio_driver.h"
#include "SPI_driver.h"
#include "C6713Helper_UdeS.h"
#include "dsk6713.h"
/****************************************************************************
	Extern content declaration :
****************************************************************************/

extern far void vectors();   // Vecteurs d'interruption

/****************************************************************************
	Private macros and constants :
****************************************************************************/

/*********************************************
Variables globales pour utiliser AIC du DSK
*********************************************/
Uint32 fs=DSK6713_AIC23_FREQ_16KHZ;  // Fréquence d'échantillonnage selon les définitions du DSK
#define DSK6713_AIC23_INPUT_LINE 0x0011
#define DSK6713_AIC23_INPUT_MIC 0x0015
Uint16 inputsource=DSK6713_AIC23_INPUT_MIC; // selection de l'entrée

#define GAUCHE 0 // Haut-parleur gauche
#define DROIT  1 // Haut-parleur droit
union {Uint32 uint; short channel[2];} AIC23_data; // Pour contenir les deux signaux

/****************************************************************************
	Private Types :
****************************************************************************/

// These type declaration are only valid in this .c

/****************************************************************************
	Private global variables :
****************************************************************************/

// Use static keyword here

/****************************************************************************
	Private functions :
****************************************************************************/

// these function can only be called by this .c
// Use static keyword

/****************************************************************************
	Public functions :
****************************************************************************/

// Function description here ...
void Audio_init(void)
{
    comm_intr(fs,inputsource);
	return;
}


/****************************************************************************
	ISR :
****************************************************************************/

interrupt void c_int11(void)
{

    short sample = (short) input_sample();

    PutChar((int) sample);

    output_sample(sample);         // Sortir les deux signaux sur HEADPHONE


}

// end of Audio_driver.c
