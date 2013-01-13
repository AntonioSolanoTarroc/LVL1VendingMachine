#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "pca9554.h"

#define BUTTONS1_IRQ 17
#define BUTTONS2_IRQ 18
#define BUTTONS3_IRQ 21

#define BUTTONS1_ADDR 0x23
#define BUTTONS2_ADDR 0x24
#define BUTTONS3_ADDR 0x25

#define I2C_BUS_DEVICE "/dev/i2c-1"

#define DEBOUNCE_TIME 100

PI_THREAD (waitForPin1)
{

  (void)piHiPri (10) ;	// Set this thread to be high priority
  
  for (;;)
  {
    if (waitForInterrupt (BUTTONS1_IRQ, -1) > 0)	// Got it
    {
		printf ("got1") ; fflush (stdout) ;
		delay(DEBOUNCE_TIME);
    }

    if (waitForInterrupt (BUTTONS2_IRQ, -1) > 0)	// Got it
    {
		printf ("got2") ; fflush (stdout) ;
		delay(DEBOUNCE_TIME);
    }

    if (waitForInterrupt (BUTTONS3_IRQ, -1) > 0)	// Got it
    {
		printf ("got3") ; fflush (stdout) ;
		delay(DEBOUNCE_TIME);
    }

  }
}

void setup(void)
{
	wiringPiSetupSys();
	piThreadCreate(waitForPin1);
}

void test_i2c(void)
{
	
	initI2c(I2C_BUS_DEVICE);

	writeConfig(BUTTONS3_ADDR, 0xCF);

	writeOutputs(BUTTONS3_ADDR, 0x00);
	delay(500);
	writeOutputs(BUTTONS3_ADDR, 0x30);

}