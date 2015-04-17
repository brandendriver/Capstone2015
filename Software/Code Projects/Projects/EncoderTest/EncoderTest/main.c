#ifndef _LINUX
#include <windows.h>
#include <conio.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef _LINUX
#include "826api.h"
#else
#include "826api.h"
#endif

static int LinearVelocity(uint brd, uint chan){
	float rad = .975;															//radius of wheel attached to the encoder in inches
	int tmax = 1000000;											// max interval time in microseconds, this will wait until event edge event is detected
	uint val = 0;
	//uint *value;
//	value = &val;
	uint timestamp, reason;

	S826_CounterSnapshotRead(brd, chan, &val, &timestamp, &reason, tmax); //Obtain a snapshot of what the counts buffer has after tmax time
										
	double pi = 3.1415;
	float numerator = val * 2 * pi * rad;									// calculate the numerator value for linear velocity
	int fullrotation = 5000;												// nuber of counts in a full rotation of the encoder
	float lin = numerator / fullrotation;									// calculates the linear velocity of wirefeed in inches/second
	printf("linear velocity = %.2f\n", lin);									// display the current linear velocity, should update every second.
}

int main(void){
	uint brd = 0;															// S826 board number set by dip switches on the board
	uint chan = 0;															// counter channel number
	uint ctrmode = 0x00008009;												// counter mode
	uint PLcount = 0;														// Pre-Load count for resetting the counter after an event
	uint preload0reg = 0;													// Value to place in the preload0 register in pre-load


	int k = 1;

	int errcode = S826_ERR_OK;

	int boardflags = S826_SystemOpen();									   // open 826 driver and find all 826 boards

	if (boardflags < 0)
		errcode = boardflags;											   // problem during open
	else if ((boardflags & (1 << brd)) == 0)
		printf("TARGET BOARD NOT FOUND\n");								   // driver didn't find board you want to use
	else
	{
		S826_CounterStateWrite(brd, chan, 0);
		S826_CounterModeWrite(brd, chan, ctrmode);						   // use the CounterModeWrite function to set the mode of the counter (in this case the mode is set to frequency measurement)
		S826_CounterSnapshotConfigWrite(brd, chan, 0x10, 0);			   // use the CounterSnapshotConfigureWrite to configure the snapshot collection on our counter
		S826_CounterPreloadWrite(brd, chan, preload0reg, PLcount);		   // use the CounterPreloadWrite function to pre-load 0 into the counts buffer.
		S826_CounterStateWrite(brd, chan, 1);
		while (k == 1){													   // begin infinite loop
			LinearVelocity(brd, chan);									   // call the linear velocity function
		}
	}
	return 0;

}