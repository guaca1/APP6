/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier SPI_driver.h
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/

#pragma once
#ifndef _SPI_MODULE_
#define _SPI_MODULE_

/***************************************************************************
	Include headers :
***************************************************************************/


// standard libraries 
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition


// #include "something.h"
#include "dsk6713.h"
#include "dsk6713_aic23.h"
/***************************************************************************
	Set EXTERN macro :
***************************************************************************/

#ifdef SPI_DRIVER_MODULE_IMPORT
	#define EXTERN  
#else 
	#define EXTERN extern
#endif

/***************************************************************************
	Constants declaration :
***************************************************************************/

//#define something something_else

/***************************************************************************
	Types declaration here :
***************************************************************************/

// typedef, struct, enum, union, etc.

/***************************************************************************
	Global variables declaration :
***************************************************************************/

// look at example for EXTERN pattern



/***************************************************************************		
	Function prototype :
***************************************************************************/

// Function description here ...
void SPI_init(void);
void interrupt_init(void);
void PutChar(int data);
int ReadChar(void);
void MCBSP_init(void);
void write_AIC(int out_data);
int test_input(int input);

// ajoutez vos prototype de fonction ici



#undef SPI_DRIVER_MODULE_IMPORT

#undef EXTERN

#endif // end of #ifndef _SPI_MODULE_

// end of SPI_driver.h
