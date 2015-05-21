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
#define MaxTurn 4222180
#define HomeOffset 1.6
#define PWMTimeConstant 88900

// PWM GENERATOR -----------------------

// Counter mode: count down at 1MHz, preload when 0 reached, use both preload registers, assert ExtOut when Preload1 is active
#define PWM_MODE  (S826_CM_K_1MHZ | S826_CM_UD_REVERSE | S826_CM_PX_ZERO | S826_CM_PX_START | S826_CM_BP_BOTH | S826_CM_OM_PRELOAD)

#define X826(FUNC)   if ((errcode = FUNC) != S826_ERR_OK) { printf("\nERROR: %d\n", errcode); return errcode;}

//#define PWM_MODE 0x01682020

// Configure a counter channel to operate as a pwm generator and start it running.
static int PwmGeneratorStart(uint board, uint ontime, uint offtime)
{
	uint counter = 5;
	int errcode;
	X826(S826_CounterStateWrite(board, counter, 0));  // halt counter channel if it's running
	X826(S826_CounterModeWrite(board, counter, PWM_MODE));  // configure counter as pwm generator
	X826(S826_CounterSnapshotConfigWrite(board, counter, 0, S826_BITWRITE));  // don't need counter snapshots -- we're just outputting pwm signal
	X826(S826_CounterPreloadWrite(board, counter, 0, offtime));  // program pwm on-time in microseconds
	X826(S826_CounterPreloadWrite(board, counter, 1, ontime));  // program pwm off-time in microseconds
	X826(S826_CounterStateWrite(board, counter, 1));  // start pwm generator
	return 0;
}


// Halt channel operating as pwm generator.
static int PwmGeneratorStop(uint board)
{
	uint counter = 5;
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

static int WireSpeed(uint brd){

	float rad = .975;															//radius of wheel attached to the encoder in inches
	int tmax = 1000000;											// max interval time in microseconds, this will wait until event edge event is detected
	uint val = 0;
	uint timestamp, reason;
	uint chan = 0;

	S826_CounterSnapshotRead(brd, chan, &val, &timestamp, &reason, tmax); //Obtain a snapshot of what the counts buffer has after tmax time

	double pi = 3.1415;
	float numerator = val  * pi * rad;									// calculate the numerator value for linear velocity
	int fullrotation = 500;												// nuber of counts in a full rotation of the encoder
	float lin = numerator / fullrotation;									// calculates the linear velocity of wirefeed in inches/second
	printf("Wire Feed = %.2f in/sec\t\n", lin);									// display the current linear velocity, should update every second.
}


static int CounterIni(uint brd){											//Function for initializing counter in frequency measurement mode
	uint chan = 0;															// counter channel number
	uint ctrmode = 0x0000800A;												// counter mode
	uint PLcount = 0;														// Pre-Load count for resetting the counter after an event
	uint preload0reg = 0;

	S826_CounterStateWrite(brd, chan, 0);								//Deactive counter
	S826_CounterModeWrite(brd, chan, ctrmode);						   // use the CounterModeWrite function to set the mode of the counter (in this case the mode is set to frequency measurement)
	S826_CounterSnapshotConfigWrite(brd, chan, 0x10, 0);			   // use the CounterSnapshotConfigureWrite to configure the snapshot collection on our counter
	S826_CounterPreloadWrite(brd, chan, preload0reg, PLcount);		   // use the CounterPreloadWrite function to pre-load 0 into the counts buffer.
	S826_CounterStateWrite(brd, chan, 1);								//Activate counter

}

static int SetWireSpeed(uint brd, double setting){
	uint time = 0;
	uint TimeStampOld = 0;



	setting = setting - HomeOffset;
	uint TimeOn = PWMTimeConstant * setting;

	PwmGeneratorStart(brd, 500, 500);
	S826_TimestampRead(brd, &TimeStampOld);		// get initial timestamp for the start of the loop

	ElapsedTime(brd, TimeStampOld, &time);		//Check the elapsed time in the loop
	while (time <= TimeOn){		//begin loop as long as elapsed time is less than 800ms

		ElapsedTime(brd, TimeStampOld, &time);		//check the elapsed time once more
		//printf("time is: %u\n", time);		//print the elapsed time
	}
	PwmGeneratorStop(brd);	//stop the pwm generator
}

//takes board number, and pointers to two different dio state arrays, returns masked values of both arrays for manipulation in main.
static int GetAndUpdateG0G1(uint brd, uint *diostatesA, uint *diostatesB, int PrintError){
	uint diostatesB_Mask[2] = { G1G0outputmask, 0 };
	uint diostatesComp[2];

	S826_DioInputRead(brd, diostatesA);	//read the digital input states
	diostatesA[0] &= G1G0inputmask;  //masking to the first two bits of the 3rd byte (channels 19 & 18)

	diostatesComp[0] = (~diostatesA[0] >> 2);		//bitshift right by two bits

	S826_DioOutputRead(brd, diostatesB);		//read the digital out states
	diostatesB[0] &= G1G0outputmask;		//mask off the last two bits of the 3rd byte (channels 17 & 16)

	if ((diostatesB[0] != diostatesComp[0])){	//check if SHIFTED input dio is the same as current output dio
		S826_DioOutputWrite(brd, diostatesB_Mask, 1);
		S826_DioOutputWrite(brd, diostatesComp, 2);		//if it's not, write the SHIFTED input to output

	}
	if (diostatesA[0] == 0xC0000); {	//check if G0G1 are in unrecognized states, i.e. 11
		//Estop();		//Stop CNC machine AND welder
		PrintError = 0;		//Set the error code for unrecognized states
		//ErrorDecode(PrintError);		//decode the error and halt program
	}

}

static int GetInputG0G1(uint brd, uint *diostatesA, int PrintError){

	S826_DioInputRead(brd, diostatesA);	//read the digital input states
	diostatesA[0] &= G1G0inputmask;  //masking to the first two bits of the 3rd byte (channels 19 & 18)

	if (diostatesA[0] == 0xC0000); {	//check if G0G1 are in unrecognized states, i.e. 11
		Estop();		//Stop CNC machine AND welder
		PrintError = 0;		//Set the error code for unrecognized states
		ErrorDecode(PrintError);		//decode the error and halt program
	}

}


int main(void){
	int rc;
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


		uint WeldOn[2] = {0x20000 , 0};
		uint WeldOff[2] = { 0x10000, 0 };
		uint direction[2] = { DirectionMask, 0 };
		uint PulsePin[2] = { PulseMask, 0 };

		uint pinstate[2] = { WireSpeedHomeMask, 0 };

		S826_DioOutputWrite(brd, pinstate, 1);

		// enable writing to safe mode registers
		rc = S826_SafeWrenWrite(brd, 0x02);
		if (rc != 0) {
			printf("failed to enable wren for watchdog\n");
			return rc;
		}

		
		S826_DioOutputSourceWrite(brd, PulsePin);
	

		CounterIni(brd);
		//set motor direction CW
		S826_DioInputRead(brd, pinstate);
		pinstate[0] &= WireSpeedHomeMask;
		if (pinstate[0] != WireSpeedHomeMask){
			S826_DioOutputWrite(brd, direction, 1);
			S826_DioOutputWrite(brd, direction, 2);
			PwmGeneratorStart(brd, counter, 5000, 5000);
			while (pinstate[0] != WireSpeedHomeMask){
				S826_DioInputRead(brd, pinstate);
				pinstate[0] &= WireSpeedHomeMask;
			}
			PwmGeneratorStop(brd, counter);
		}

		S826_DioOutputWrite(brd, direction, 1);
		//S826_DioOutputWrite(brd, direction, 2);
		//Turn welder on
		//Sleep(500);
		//S826_DioOutputWrite(brd, WeldOn, 0);

		//Sleep(100);
		//read encoder
		//S826_TimestampRead(brd, &TimeStampOld);
		//ElapsedTime(brd, TimeStampOld, &time);		//Check the elapsed time in the loop

		//while (time <= 3000000){		//begin loop as long as elapsed time is less than 800ms
			//WireSpeed(brd);
			//ElapsedTime(brd, TimeStampOld, &time);		//check the elapsed time once more
		//}
		
		//turn welder off

		//S826_DioOutputWrite(brd, WeldOff, 0);

		Sleep(1000);


		//turn motor 10deg
		SetWireSpeed(brd, 7);

		/*
		Sleep(4000);
		S826_DioInputRead(brd, pinstate);
		pinstate[0] &= WireSpeedHomeMask;
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

	//	S826_DioOutputWrite(brd, direction, 1);
	//	SetWireSpeed(brd, 4);
	//	SetWireSpeed(brd, 6);

	*/
		Sleep(1000);

		//turn welder on
		//S826_DioOutputWrite(brd, WeldOn, 0);

		//Sleep(100);
		//read encoder
		//S826_TimestampRead(brd, &TimeStampOld);
		//ElapsedTime(brd, TimeStampOld, &time);
		//while (time <= 3000000){		//begin loop as long as elapsed time is less than 800ms
			//WireSpeed(brd);
			//ElapsedTime(brd, TimeStampOld, &time);		//check the elapsed time once more
		//}
		//turn welder off
		//S826_DioOutputWrite(brd, WeldOff, 0);



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