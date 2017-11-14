/********************************************************
**  Session 5 - APP6 - T�l�phonie par DSP
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


#include "Audio_driver.h"
#include "dsk6713.h"




/***************************************************************************
    Include Module Header :
***************************************************************************/
#define EXTPOL (unsigned int*) 0x019C0008

#define SPI_DRIVER_MODULE_IMPORT
#define SPI_WRITE_CONFIG 0xE440
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
 * B3�B0 = XXXX, Baud-Rate Divisor Select Bits = 230.4kBaud
 */
#define SPI_READ_CONFIG 0x4000
/*
 * 0 = Read Configuration
 * 1 = Read Configuration
 */
#define SPI_WRITE_DATA 0x8000
#define SPI_READ_DATA 0x0000
#define EXTPOL (unsigned int*) 0x019C0008


/****************************************************************************
    Extern content declaration :
****************************************************************************/

extern far void vectors();   // Vecteurs d'interruption
extern MCBSP_Handle DSK6713_AIC23_CONTROLHANDLE;
extern MCBSP_Handle test_mcbsp;

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
    data = data & 0x00FF;
    int temp = SPI_WRITE_DATA | data;
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, temp);
}

int ReadChar()
{
    while(!MCBSP_xrdy(DSK6713_AIC23_CONTROLHANDLE));
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, SPI_READ_DATA);
    while(!MCBSP_rrdy(DSK6713_AIC23_CONTROLHANDLE));
    int test = (MCBSP_read(DSK6713_AIC23_CONTROLHANDLE) & 0x00FF);
    return test;

}
// these function can only be called by this .c
// Use static keyword

/****************************************************************************
    Public functions :
****************************************************************************/

// Function description here ...
void SPI_init(void)
{
    DSK6713_rset(DSK6713_MISC,DSK6713_rget(DSK6713_MISC) | 0x1);
    int data = MCBSP_read(DSK6713_AIC23_CONTROLHANDLE);
    data = 0;

 while((data & 0x3FFF)!=(SPI_WRITE_CONFIG & 0x3FFF))
 {

    while(!MCBSP_xrdy(DSK6713_AIC23_CONTROLHANDLE));
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE,(int)SPI_WRITE_CONFIG);

    while(!MCBSP_rrdy(DSK6713_AIC23_CONTROLHANDLE));
    data = MCBSP_read(DSK6713_AIC23_CONTROLHANDLE);       //On devrait pouvoir valider notre config

    while(!MCBSP_xrdy(DSK6713_AIC23_CONTROLHANDLE));
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE,SPI_READ_CONFIG);

    while(!MCBSP_rrdy(DSK6713_AIC23_CONTROLHANDLE));
    data = MCBSP_read(DSK6713_AIC23_CONTROLHANDLE);       //On devrait pouvoir valider notre config

 }

    IRQ_clear(IRQ_EVT_EXTINT4);
    interrupt_init();

    return;
}


void MCBSP_init()
{
    MCBSP_Config MCBSP0_SPI_Cfg = {
                MCBSP_FMKS(SPCR, FREE, NO)              |
                MCBSP_FMKS(SPCR, SOFT, NO)              |
                MCBSP_FMKS(SPCR, FRST, OF(0))             |
                MCBSP_FMKS(SPCR, GRST, OF(0))             |
                MCBSP_FMKS(SPCR, XINTM, XRDY)           |
                MCBSP_FMKS(SPCR, XSYNCERR, NO)          |
                MCBSP_FMKS(SPCR, XRST, OF(0))              | //Mode reset si 1
                MCBSP_FMKS(SPCR, DLB, OFF)              |
                MCBSP_FMKS(SPCR, RJUST, RZF)            |
                MCBSP_FMKS(SPCR, CLKSTP, DELAY)         | //MET LE EN COMMENTAIRE AU PIRE
                MCBSP_FMKS(SPCR, DXENA, OFF)            |
                MCBSP_FMKS(SPCR, RINTM, RRDY)           |
                MCBSP_FMKS(SPCR, RSYNCERR, NO)          |
                MCBSP_FMKS(SPCR, RRST, OF(0)),               //Mode reset si 1

                MCBSP_FMKS(RCR, RPHASE, SINGLE)         |
                MCBSP_FMKS(RCR, RFRLEN2, OF(0))       |
                MCBSP_FMKS(RCR, RWDLEN2, 16BIT)       |
                MCBSP_FMKS(RCR, RCOMPAND, MSB)          |
                MCBSP_FMKS(RCR, RFIG, NO)               |
                MCBSP_FMKS(RCR, RDATDLY, 1BIT)          | //* an XDATDLY value of 0 or 2 causes undefined operation and an RDATDLY value of 0 causes the received data to be shifted incorrectly.
                MCBSP_FMKS(RCR, RFRLEN1, OF(0))         | // This changes to 1 FRAME
                MCBSP_FMKS(RCR, RWDLEN1, 16BIT)         | //* This changes to 16 bits per frame
                MCBSP_FMKS(RCR, RWDREVRS, DISABLE),

                MCBSP_FMKS(XCR, XPHASE, SINGLE)         |
                MCBSP_FMKS(XCR, XFRLEN2,OF(0))       |
                MCBSP_FMKS(XCR, XWDLEN2, 16BIT)       |
                MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
                MCBSP_FMKS(XCR, XFIG, NO)               |
                MCBSP_FMKS(XCR, XDATDLY, 1BIT)          | //* an XDATDLY value of 0 or 2 causes undefined operation and an RDATDLY value of 0 causes the received data to be shifted incorrectly.
                MCBSP_FMKS(XCR, XFRLEN1, OF(0))         | // This changes to 1 FRAME
                MCBSP_FMKS(XCR, XWDLEN1, 16BIT)         | //* This changes to 16 bits per frame
                MCBSP_FMKS(XCR, XWDREVRS, DISABLE),

                MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |
                MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |
                MCBSP_FMKS(SRGR, CLKSM, DEFAULT)        | //*Default = CLKSM_INTERNAL.
                MCBSP_FMKS(SRGR, FSGM, OF(0))           | // Default = 0, DXR-to-XSR transfer of each element generates the slave enable FSX (FSGM = 0 in SRGR).
                MCBSP_FMKS(SRGR, FPER, DEFAULT)         |
                MCBSP_FMKS(SRGR, FWID, DEFAULT)         |
                MCBSP_FMKS(SRGR, CLKGDV, OF(27)),        // 225MHZ/2/28

                MCBSP_MCR_DEFAULT,
                MCBSP_RCER_DEFAULT,
                MCBSP_XCER_DEFAULT,

                MCBSP_FMKS(PCR, XIOEN, SP)              | //SP = DX, FSX, and CLKX pins are configured as serial port pins and do not function as general-purpose I/O pins.
                MCBSP_FMKS(PCR, RIOEN, SP)              | //SP = DX, FSX, and CLKX pins are configured as serial port pins and do not function as general-purpose I/O pins.
                MCBSP_FMKS(PCR, FSXM, OF(1))         | //*FSX should be configured as an output that can be connected to the slave enable(/SS) input on the slave device (FSXM = 1)
                MCBSP_FMKS(PCR, FSRM, OF(1))         |
                MCBSP_FMKS(PCR, CLKXM, OF(1))          | //*CLKX should be configured as an OUTPUT in master mode
                MCBSP_FMKS(PCR, CLKRM, OF(1))           |
                MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
                MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
                MCBSP_FMKS(PCR, FSXP, ACTIVELOW)        |
                MCBSP_FMKS(PCR, FSRP, ACTIVEHIGH)       |
                MCBSP_FMKS(PCR, CLKXP, RISING)         |
                MCBSP_FMKS(PCR, CLKRP, FALLING)
            };

    MCBSP_close(DSK6713_AIC23_CONTROLHANDLE);
    DSK6713_AIC23_CONTROLHANDLE=MCBSP_open(MCBSP_DEV0,MCBSP_OPEN_RESET);
    MCBSP_config(DSK6713_AIC23_CONTROLHANDLE,&MCBSP0_SPI_Cfg);                    //interface 16 bits to MAX3111E
    MCBSP_start(DSK6713_AIC23_CONTROLHANDLE, MCBSP_SRGR_START|MCBSP_XMIT_START|MCBSP_RCV_START|MCBSP_SRGR_FRAMESYNC, 0x3000); //START GRST
    while(MCBSP_xempty(DSK6713_AIC23_CONTROLHANDLE));


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
    *EXTPOL|= 0x1;
    IRQ_nmiEnable();
    IRQ_globalEnable();

    output_sample(0);


    return;
}
// end of SPI_driver.c
