#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "826api.h"

static int LinearVelocity(uint brd, uint chan){
	uint rad = 1;															//radius of wheel attached to the encoder in inches
	uint val = 0;
	uint *value;															//create pointer to direct counts buffer value to val
	value = &val;

	uint tstamp = NULL;														//set null to ignore the timestamp	
	uint *timestamp;
	timestamp = &tstamp;

	uint rsn = NULL;														//set null to ignore the reason
	uint *reason;
	reason = &rsn;

	int tmax = S826_WAIT_INFINITE;											// max interval time in microseconds, this will wait until event edge event is detected
	S826_CounterSnapshotRead(brd, chan, &value, &timestamp, &reason, tmax); //Obtain a snapshot of what the counts buffer has after tmax time

	val = *value;															//begin calculating Linear Velocity	
	double pi = 3.1415;
	float numerator = val * 2 * pi * rad;									// calculate the numerator value for linear velocity
	int fullrotation = 5000;												// nuber of counts in a full rotation of the encoder
	float lin = numerator / fullrotation;									// calculates the linear velocity of wirefeed in inches/second
	pintf("linear velocity = %.2f\n", lin);									// display the current linear velocity, should update every second.
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
		S826_CounterModeWrite(brd, chan, ctrmode);						   // use the CounterModeWrite function to set the mode of the counter (in this case the mode is set to frequency measurement)
		S826_CounterSnapshotConfigWrite(brd, chan, 0x10, 0);			   // use the CounterSnapshotConfigureWrite to configure the snapshot collection on our counter
		S826_CounterPreloadWrite(brd, chan, preload0reg, PLcount);		   // use the CounterPreloadWrite function to pre-load 0 into the counts buffer.
		while (k == 1){													   // begin infinite loop
			LinearVelocity(brd, chan);									   // call the linear velocity function
		}
	}
	return 0;

}