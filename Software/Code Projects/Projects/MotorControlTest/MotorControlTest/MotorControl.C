#ifndef _LINUX
#include <windows.h>
#include <conio.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <windows.h>

#ifndef _LINUX
#include "826api.h"
#else
#include "826api.h"
#endif

// Helpful macros for DIOs
#define DIO(C)                  ((uint64)1 << (C))                          // convert dio channel number to uint64 bit mask
#define DIOMASK(N)              {(uint)(N) & 0xFFFFFF, (uint)((N) >> 24)}   // convert uint64 bit mask to uint[2] array
#define DIOSTATE(STATES,CHAN)   ((STATES[CHAN / 24] >> (CHAN % 24)) & 1)    // extract dio channel's boolean state from uint[2] array

#define G1G0inputmask 0x000C0000
#define G1G0outputmask 0x00030000
#define DirectionMask 0x00400000
#define WireSpeedHomeMask 0x00100000
#define PulseMask 0x00200000

// PWM GENERATOR -----------------------

// Counter mode: count down at 1MHz, preload when 0 reached, use both preload registers, assert ExtOut when Preload1 is active
#define PWM_MODE  (S826_CM_K_1MHZ | S826_CM_UD_REVERSE | S826_CM_PX_ZERO | S826_CM_PX_START | S826_CM_BP_BOTH | S826_CM_OM_PRELOAD)

//#define PWM_MODE 0x01682020

// Configure a counter channel to operate as a pwm generator and start it running.
static int PwmGeneratorStart(uint board, uint counter, uint ontime, uint offtime)
{
	//int errcode;
	S826_CounterStateWrite(board, counter, 0);  // halt counter channel if it's running
	S826_CounterModeWrite(board, counter, PWM_MODE);  // configure counter as pwm generator
	S826_CounterSnapshotConfigWrite(board, counter, 0, S826_BITWRITE);  // don't need counter snapshots -- we're just outputting pwm signal
	S826_CounterPreloadWrite(board, counter, 0, offtime);  // program pwm on-time in microseconds
	S826_CounterPreloadWrite(board, counter, 1, ontime);  // program pwm off-time in microseconds
	S826_CounterStateWrite(board, counter, 1);  // start pwm generator
	return 0;
}

// Halt channel operating as pwm generator.
static int PwmGeneratorStop(uint board, uint counter)		
{
	return S826_CounterStateWrite(board, counter, 0);
}

static int ElapsedTime(uint board, uint TimeStampOld, uint *time){
	uint64_t Tmax = 4294967296;
	uint TimeStampNew;

	S826_TimestampRead(board, &TimeStampNew);
	if (TimeStampOld > TimeStampNew){
		*time = Tmax - TimeStampOld + TimeStampNew;
	}
	else{
		*time = (TimeStampNew - TimeStampOld); // / 1000;
		//printf("time is: %u", time);
	}

}


int main(void){
	uint brd = 0;                        // change this if you want to use other than board number 0
	uint diochan1 = 21;					//initialize our DIO channel
	uint diochan2 = 22;
	uint diomask1[] = DIOMASK(DIO(diochan1));		//mask the DIO channel
	uint diomask2[] = DIOMASK(DIO(diochan2));
	uint counter = 5;		//initialize counter channel variable
	int k = 1;		//infinite loop variable
	uint TimeStampOld = 0;		//initialize timestamp
	uint time = 0;		//iniltialize time


	int errcode = S826_ERR_OK;
	int boardflags = S826_SystemOpen();        // open 826 driver and find all 826 boards

	if (boardflags < 0)
		errcode = boardflags;                       // problem during open
	else if ((boardflags & (1 << brd)) == 0)
		printf("TARGET BOARD NOT FOUND\n");         // driver didn't find board you want to use
	//////////////////////////////////////////////////////////////////////////////////
	//////////Main Loop Starts Here///////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	else
	{
		
		
		S826_DioOutputSourceWrite(brd, diomask1);  // route counter's ExtOut signal to dio pin
		while (k == 1){				//begin infinte loop for testing

			S826_DioOutputWrite(brd, diomask2, 1);
			S826_DioOutputWrite(brd, diomask2, 2);
			
			PwmGeneratorStart(brd, counter, 500, 500);  // start pwm generator with 300ms/700ms on/off times
			S826_TimestampRead(brd, &TimeStampOld);		// get initial timestamp for the start of the loop
	
			ElapsedTime(brd, TimeStampOld, &time);		//Check the elapsed time in the loop
			while (time <= 800000){		//begin loop as long as elapsed time is less than 800ms

				ElapsedTime(brd, TimeStampOld, &time);		//check the elapsed time once more
				printf("time is: %u\n", time);		//print the elapsed time
			}
		PwmGeneratorStop(brd, counter);	//stop the pwm generator
		Sleep(1000);	//short sleep cycle as a delay
		time = 0;

		S826_DioOutputWrite(brd, diomask2, 1);
		PwmGeneratorStart(brd, counter, 500, 500);  // start pwm generator with 300ms/700ms on/off times
		S826_TimestampRead(brd, &TimeStampOld);		// get initial timestamp for the start of the loop

		ElapsedTime(brd, TimeStampOld, &time);		//Check the elapsed time in the loop
		while (time <= 800000){		//begin loop as long as elapsed time is less than 800ms

			ElapsedTime(brd, TimeStampOld, &time);		//check the elapsed time once more
			printf("time is: %u\n", time);		//print the elapsed time
		}
		PwmGeneratorStop(brd, counter);	//stop the pwm generator
		Sleep(1000);	//short sleep cycle as a delay
		time = 0;
		
	}
		/*
		uint pinstate[2];
		uint direction[2] = { DirectionMask, 0 };
		uint PulsePin[2] = { PulseMask, 0 };

		S826_DioInputRead(brd, pinstate);
		pinstate[0] &= WireSpeedHomeMask;
		S826_DioOutputSourceWrite(brd, PulsePin);

		if (pinstate[0] != 0x100000){
			S826_DioOutputWrite(brd, direction, 1);
			S826_DioOutputWrite(brd, direction, 2);
			PwmGeneratorStart(brd, counter, 5000, 5000);
			while (pinstate[0] != 0x100000){
				S826_DioInputRead(brd, pinstate);
				pinstate[0] &= WireSpeedHomeMask;
			}
			PwmGeneratorStop(brd, counter);
		}

		*/
		
	}

	switch (errcode)
	{
	case S826_ERR_OK:           break;
	case S826_ERR_BOARD:        printf("Illegal board number"); break;
	case S826_ERR_VALUE:        printf("Illegal argument"); break;
	case S826_ERR_NOTREADY:     printf("Device not ready or timeout"); break;
	case S826_ERR_CANCELLED:    printf("Wait cancelled"); break;
	case S826_ERR_DRIVER:       printf("Driver call failed"); break;
	case S826_ERR_MISSEDTRIG:   printf("Missed adc trigger"); break;
	case S826_ERR_DUPADDR:      printf("Two boards have same number"); break; S826_SafeWrenWrite(brd, 0x02);
	case S826_ERR_BOARDCLOSED:  printf("Board not open"); break;
	case S826_ERR_CREATEMUTEX:  printf("Can't create mutex"); break;
	case S826_ERR_MEMORYMAP:    printf("Can't map board"); break;
	default:                    printf("Unknown error"); break;
	}


#ifndef _LINUX	
	printf("\nKeypress to exit ...\n\n");
	while (!_kbhit());
	_getch();
#endif

	S826_SystemClose();
	return 0;
}