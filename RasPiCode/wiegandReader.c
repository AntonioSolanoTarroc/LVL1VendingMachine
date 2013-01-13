#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sched.h>

#include <wiringPi.h>

#include "wiegandReader.h"

#define D0_PIN 23
#define D1_PIN 24

#define MAX_BITS 100                 // max number of bits 
#define WIEGAND_WAIT_TIME 300000      // time to wait for another wiegand pulse.  

#define MAX_CALLBACKS 10

static cardHandler cardCallbacks[MAX_CALLBACKS] = {NULL};
static int currentCallbacks = 0;

static unsigned char databits[MAX_BITS];    // stores all of the data bits
static unsigned char bitCount;              // number of bits currently captured
static unsigned int flagDone;              // goes low when data is currently being captured
static unsigned int wiegand_counter;        // countdown until we assume there are no more bits

static unsigned long cardCode=0;            // decoded card code

static int setHighPriority(int priority)
{
  struct sched_param schedule;

  memset(&schedule, 0, sizeof(schedule));

  if (priority > sched_get_priority_max (SCHED_RR))
    priority = sched_get_priority_max(SCHED_RR);

  schedule.sched_priority = priority ;

  return sched_setscheduler(0, SCHED_RR, &schedule);
}

static void *waitForData0(void *arg)
{
	(void)arg;

	setHighPriority(10);
	while(1)
	{
	    if (waitForInterrupt (D0_PIN, -1) > 0)
	    {
			bitCount++;
			flagDone = 0;
			wiegand_counter = WIEGAND_WAIT_TIME;  
	    }
	}

	return NULL;
}

static void *waitForData1(void *arg)
{
	(void)arg;

	setHighPriority(10);
	while(1)
	{
		if (waitForInterrupt (D1_PIN, -1) > 0)
	    {
			databits[bitCount] = 1;
			bitCount++;
			flagDone = 0;
			wiegand_counter = WIEGAND_WAIT_TIME;  
	    }	
	}

	return NULL;
}

static void printBits(void)
{
      // Prints out the results
      printf ("Read %d bits\n", bitCount); fflush (stdout);
      printf ("TagID: %lx\n\n", cardCode<<1); fflush (stdout); 
	  return;
}

static void *cardReader(void * arg)
{
	(void)arg;

	while(1)
	{
		if(!flagDone)
		{
			if(--wiegand_counter == 0)
			{
				flagDone = 1;
			}
		}

		if(bitCount > 0 && flagDone)
		{
			if(bitCount == 26) //Use 26 bit card codes
			{
				for(int i=0; i < 25; i++)
				{
					cardCode <<= 1;
					cardCode |= databits[i];
				}

				for(int i = 0; i < currentCallbacks; i++)
				{
					(*cardCallbacks[i])(cardCode);
				}
				//printBits();
			}

			bitCount = 0;
			cardCode = 0;
			
			for (int i=0; i<MAX_BITS; i++) 
			{
			  databits[i] = 0;
			}
		}



	}

	return NULL;
}

void addCardCallback(cardHandler pCallback)
{
	if(currentCallbacks < MAX_CALLBACKS)
		cardCallbacks[currentCallbacks++] = pCallback;
}

pthread_t data0_thread, data1_thread, reader_thread;

void startCardReaderThreads(void)
{
	pthread_create(&data0_thread, NULL, waitForData0, NULL);
	pthread_create(&data1_thread, NULL, waitForData1, NULL);
	pthread_create(&reader_thread, NULL, cardReader, NULL);

}
