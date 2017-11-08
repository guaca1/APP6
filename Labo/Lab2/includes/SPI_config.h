/*
 * SimpleInterrupt.h
 *
 *  Created on: Nov 8, 2017
 *      Author: danae
 */

#include "csl_mcbsp.h"
#include "csl_mcbsphal.h"
#include "csl.h"
#include "csl_gpio.h"
#include "csl_irq.h"
#include "dsk6713.h"
#include "dsk6713_led.h"
#include "C6713dskinit.h"

#define Uint32 int
#ifndef INCLUDES_SPI_CONFIG_H_
#define INCLUDES_SPI_CONFIG_H_



MCBSP_Config MCBSP0_SPI_Cfg = {
                               MCBSP_FMKS(SPCR, XRST, 0)                                            |
                               MCBSP_FMKS(SPCR, RRST, 0)                                            |
                               MCBSP_FMKS(SPCR, CLKSTP, NODELAY)                  |
                               MCBSP_FMKS(SPCR, GRST, NO)                           ,
                               MCBSP_RCR_DEFAULT,
                               MCBSP_XCR_DEFAULT,
                               MCBSP_SRGR_DEFAULT,
                               MCBSP_MCR_DEFAULT,
                               MCBSP_RCER_DEFAULT,
                               MCBSP_XCER_DEFAULT,
                               MCBSP_FMKS(PCR, CLKXP, RISING)                       |
                               MCBSP_FMKS(PCR, CLKXM, INPUT)                        ,
};

interrupt void c_int04();

#endif /* INCLUDES_SPI_CONFIG_H_ */
