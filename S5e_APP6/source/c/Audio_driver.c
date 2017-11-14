/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier principal Telephone.c
**  Auteurs : < LANF1601, DESK1902, SOUC2303, DUFD2703 >
**  Date : < 13/11/17 >
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

extern char FLAG_LECTURE;
extern char FLAG_ECRITURE;
extern char FLAG_COMP;
extern char FLAG_ENRG;

extern short outputSample;

unsigned char int2ulaw(short linear);
int ulaw2int(unsigned char log);
extern short enregistrement[160000];
extern char FLAG_REJOUE;
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

int traiter_output(int output)
{

    if (!FLAG_COMP)
    {
        return (output << 8);
    }
    else
    {
        int test = (ulaw2int((unsigned char)output) << 2);
        return (test);
    }

}


int traiter_input(int input)
{
    int retVal;
    if (!FLAG_COMP)
    {
        retVal =  (input >> 8);
    }
    else
    {
        retVal = (int2ulaw(((short)input)>>2));

    }
    return ((retVal>>1)<<1) + FLAG_ENRG;
}
/****************************************************************************
	ISR :
****************************************************************************/

interrupt void c_int11(void)
{
    int output = traiter_output(outputSample);
    static int compteurRejouage = 0;
    FLAG_LECTURE = true;
    if(FLAG_REJOUE)
    {
        output_sample((output<<16)+traiter_output(enregistrement[compteurRejouage++]));
        if(compteurRejouage >= 160000)
        {
            compteurRejouage = 0;
            FLAG_REJOUE = false;
        }
    }
    else
    {
        output_sample((output<<16)+output);
    }

}

interrupt void c_int04(void)
{
    FLAG_ECRITURE = true;
    return;
}

// end of Audio_driver.c
