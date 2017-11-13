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
#include "csl_mcbsp.h"
#include "csl_gpio.h"
#include "C6713Helper_UdeS.h"
#include "csl_irq.h"
#include "SPI_driver.h"
#include "dsk6713.h"



/***************************************************************************
	Include Module Header :
***************************************************************************/

#define SPI_DRIVER_MODULE_IMPORT
#define SPI_WRITE_CONFIG 0xE040
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
#define SPI_READ_CONFIG 0x4000
/*
 * 0 = Read Configuration
 * 1 = Read Configuration
 */
#define SPI_WRITE_DATA 0x8000
#define SPI_READ_DATA 0x0000



/****************************************************************************
	Extern content declaration :
****************************************************************************/

extern far void vectors();   // Vecteurs d'interruption
extern MCBSP_Handle DSK6713_AIC23_CONTROLHANDLE;
extern MCBSP_Handle test_mcbsp

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
    data = data & 0x000F;
    int temp = SPI_WRITE_DATA | data;
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, temp);
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
    int temp;
    char isReady = false;
    while(1)
    {
        MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, SPI_WRITE_CONFIG);
        temp = MCBSP_read(DSK6713_AIC23_CONTROLHANDLE);

        if(temp == SPI_WRITE_CONFIG)
            {
                isReady = true;
            }

    }

    IRQ_clear(IRQ_EVT_EXTINT4);
    interrupt_init();

	return;
}

void MCBSP_init()
{
MCBSP_Config MCBSP0_SPI_Cfg = {
                                       MCBSP_FMKS(SPCR, FRST, YES)             |
                                       MCBSP_FMKS(SPCR, GRST, YES)             |
                                       MCBSP_FMKS(SPCR, XINTM, XRDY)           |
                                       MCBSP_FMKS(SPCR, XSYNCERR, NO)          |
                                       MCBSP_FMKS(SPCR, XRST, YES)             |
                                       MCBSP_FMKS(SPCR, DLB, OFF)              |
                                       MCBSP_FMKS(SPCR, RJUST, RZF)            |
                                       MCBSP_FMKS(SPCR, CLKSTP, DISABLE)       |
                                       MCBSP_FMKS(SPCR, DXENA, OFF)            |
                                       MCBSP_FMKS(SPCR, RINTM, RRDY)           |
                                       MCBSP_FMKS(SPCR, RSYNCERR, NO)          |
                                       MCBSP_FMKS(SPCR, RRST, YES),

                                       MCBSP_FMKS(RCR, RPHASE, SINGLE)         |
                                       MCBSP_FMKS(RCR, RFRLEN2, DEFAULT)       |
                                       MCBSP_FMKS(RCR, RWDLEN2, 16BIT)       |
                                       MCBSP_FMKS(RCR, RCOMPAND, MSB)          |
                                       MCBSP_FMKS(RCR, RFIG, NO)               |
                                       MCBSP_FMKS(RCR, RDATDLY, 1BIT)          |
                                       MCBSP_FMKS(RCR, RFRLEN1, OF(0))         | // This changes to 1 FRAME
                                       MCBSP_FMKS(RCR, RWDLEN1, 32BIT)         | // This changes to 32 bits per frame
                                       MCBSP_FMKS(RCR, RWDREVRS, DISABLE),

                                       MCBSP_FMKS(XCR, XPHASE, SINGLE)         |
                                       MCBSP_FMKS(XCR, XFRLEN2, DEFAULT)       |
                                       MCBSP_FMKS(XCR, XWDLEN2, 16BIT)       |
                                       MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
                                       MCBSP_FMKS(XCR, XFIG, NO)               |
                                       MCBSP_FMKS(XCR, XDATDLY, 1BIT)          |
                                       MCBSP_FMKS(XCR, XFRLEN1, OF(0))         | // This changes to 1 FRAME
                                       MCBSP_FMKS(XCR, XWDLEN1, 32BIT)         | // This changes to 32 bits per frame
                                       MCBSP_FMKS(XCR, XWDREVRS, DISABLE),

                                       MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |
                                       MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |
                                       MCBSP_FMKS(SRGR, CLKSM, DEFAULT)        |
                                       MCBSP_FMKS(SRGR, FSGM, DEFAULT)         |
                                       MCBSP_FMKS(SRGR, FPER, DEFAULT)         |
                                       MCBSP_FMKS(SRGR, FWID, DEFAULT)         |
                                       MCBSP_FMKS(SRGR, CLKGDV, DEFAULT),

                                       MCBSP_MCR_DEFAULT,
                                       MCBSP_RCER_DEFAULT,
                                       MCBSP_XCER_DEFAULT,

                                       MCBSP_FMKS(PCR, XIOEN, SP)              |
                                       MCBSP_FMKS(PCR, RIOEN, SP)              |
                                       MCBSP_FMKS(PCR, FSXM, INTERNAL)         |
                                       MCBSP_FMKS(PCR, FSRM, EXTERNAL)         |
                                       MCBSP_FMKS(PCR, CLKXM, OUTPUT)           |
                                       MCBSP_FMKS(PCR, CLKRM, INPUT)           |
                                       MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
                                       MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
                                       MCBSP_FMKS(PCR, FSXP, ACTIVEHIGH)       |
                                       MCBSP_FMKS(PCR, FSRP, ACTIVEHIGH)       |
                                       MCBSP_FMKS(PCR, CLKXP, FALLING)         |
                                       MCBSP_FMKS(PCR, CLKRP, RISING)
                                   };
    MCBSP_close(DSK6713_AIC23_CONTROLHANDLE);
    MCBSP_open(MCBSP_DEV0, MCBSP_OPEN);
    MCBSP_config(test_mcbspt,&MCBSP0_SPI_Cfg);
}
/****************************************************************************
	ISR :
****************************************************************************/
void interrupt_init(void)
{
    GPIO_Handle hGPIO = GPIO_open(GPIO_DEV0, GPIO_OPEN_RESET);
    GPIO_pinEnable(hGPIO, GPIO_PIN4);
    GPIO_pinDirection(hGPIO, GPIO_PIN4, GPIO_INPUT);
    GPIO_intPolarity(hGPIO, GPIO_PIN4, GPIO_RISING);


    IRQ_setVecs(vectors);
    IRQ_map(IRQ_EVT_EXTINT4, 4);
    IRQ_reset(IRQ_EVT_EXTINT4);
    IRQ_enable(IRQ_EVT_EXTINT4);
    IRQ_nmiEnable();
    IRQ_globalEnable();

    return;
}
// end of SPI_driver.c
