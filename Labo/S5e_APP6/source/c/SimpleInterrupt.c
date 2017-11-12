<<<<<<< HEAD
#include "SimpleInterrupt.h"

void main()
{
    extern far void Vectors();
    init_ext_intr();

    c_int04();

}

void init_ext_intr(void)
{

}

void c_int04(void)
{

}
=======
#include "SimpleInterrupt.h"
#include "csl.h"
#include "csl_gpio.h"
#include "csl_irq.h"
#include "dsk6713.h"
#include "dsk6713_led.h"

extern void vectors();
char flagInterrupt = 0;

static GPIO_Handle hGPIO;
static Uint32 valGPIO9 = 0;

void main()
{
    DSK6713_init();
    DSK6713_LED_init();
    init_ext_intr();
    while(1)
    {
        if(flagInterrupt)
        {
            GPIO_pinWrite(hGPIO, GPIO_PIN9, valGPIO9);
            valGPIO9 = !valGPIO9;
            flagInterrupt = 0;
        }
    }
}

void init_ext_intr()
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

    //GPIO9

    GPIO_pinEnable(hGPIO, GPIO_PIN9);
    GPIO_pinDirection(hGPIO, GPIO_PIN9, GPIO_OUTPUT);

}
interrupt void c_int04()
{
    flagInterrupt = 1;
}
>>>>>>> 940608c27ce041bb34ad9ec8081f5e0f6cb93465
