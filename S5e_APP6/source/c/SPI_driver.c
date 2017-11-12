/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier SPI_driver.c
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

#define SPI_DRIVER_MODULE_IMPORT
#define SPI_WRITE_CONFIG 0b1110000001000000
/*
 * 1 = Write Configuration
 * 1 = Write Configuration
 * 1, FIFO is disabled
 * 0, Exit software shutdown
 * 0, Transmit buffer empty interrupt is disabled
 * 0, Data available in the receive register or FIFO interrupt is disabled
 * 0, Parity bit received interrupt is disabled
 * 0,  Receiver-activity  (shutdown  mode)/Framing-error (normal operation) interrupt is disabled
 * 0, IrDA mode is disabled
 * 1, Transmit two stop-bits
 * 0, Parity is disabled for both transmit and receive
 * 0, 8-bit words
 * B3–B0 = XXXX, Baud-Rate Divisor Select Bits = 230.4kBaud
 */
#define SPI_READ_CONFIG 0b0100000000000000
/*
 * 0 = Read Configuration
 * 1 = Read Configuration
 */
#define SPI_WRITE_DATA 0x8000
#define SPI_READ_DATA 0x0000

#include "SPI_driver.h"

/****************************************************************************
	Extern content declaration :
****************************************************************************/

extern far void vectors();   // Vecteurs d'interruption

/****************************************************************************
	Private macros and constants :
****************************************************************************/

// These defines are only valid is this .c

//#define something somethingelse

/****************************************************************************
	Private Types :
****************************************************************************/

// These type declaration are only valid in this .c

/****************************************************************************
	Private global variables :
****************************************************************************/


/****************************************************************************
	Private functions :
****************************************************************************/

void PutChar(int data)
{
    data = data && 0x000F;
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, SPI_WRITE_DATA | data);
}

int ReadChar()
{
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, SPI_READ_DATA);
    return MCBSP_read(DSK6713_AIC23_CONTROLHANDLE);

}
// these function can only be called by this .c
// Use static keyword

/****************************************************************************
	Public functions :
****************************************************************************/

// Function description here ...
void SPI_init(void)
{
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, SPI_WRITE_CONFIG);

    while (!MCBSP_xrdy() )
    {
        DSK6713_waitusec(10);
    }
    int temp = ReadChar();
    IRQ_clear();
    interrupt_init();

	return;
}


/****************************************************************************
	ISR :
****************************************************************************/
void interrup_init(void)
{
    hGPIO = GPIO_open(GPIO_DEV0, GPIO_OPEN_RESET);
    GPIO_pinEnable(hGPIO, GPIO_PIN4);
    GPIO_pinDirection(hGPIO, GPIO_PIN4, GPIO_INPUT);
    GPIO_intPolarity(hGPIO, GPIO_PIN4, GPIO_RISING);


    IRQ_setVecs(vectors);
    IRQ_map(IRQ_EVT_EXTINT4, 4);
    IRQ_reset(IRQ_EVT_EXTINT4);
    IRQ_enable(IRQ_EVT_EXTINT4);
    IRQ_nmiEnable();
    IRQ_globalEnable();
}
// end of SPI_driver.c
