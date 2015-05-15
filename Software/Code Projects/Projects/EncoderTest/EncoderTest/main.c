//This program is free software : you can redistribute it and / or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////

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


#define X826(FUNC)   if ((errcode = FUNC) != S826_ERR_OK) { printf("\nERROR: %d\n", errcode); return errcode;}

static int LinearVelocity(uint brd, uint chan, uint *timestamp){
	float diameter = .975;															//radius of wheel attached to the encoder in inches
	int tmax = 1000000;											// max interval time in microseconds, this will wait until event edge event is detected
	uint val = 0;
	//uint *value;
//	value = &val;
	uint reason;


	S826_CounterSnapshotRead(brd, chan, &val, &timestamp, &reason, tmax); //Obtain a snapshot of what the counts buffer has after tmax time
										
	double pi = 3.1415;
	float numerator = val  * pi * diameter;									// calculate the numerator value for linear velocity
	int fullrotation = 500;												// nuber of counts in a full rotation of the encoder
	float lin = numerator / fullrotation;									// calculates the linear velocity of wirefeed in inches/second
	printf("linear velocity = %.2f\n", lin);									// display the current linear velocity, should update every second.
}

static int ElapsedTime(uint board, uint TimeStampOld, uint *time){		//takes board number, a time stamp from main, and a pointer to a time value in main, checks for time overflow and returns an elapsed time.
	uint64_t Tmax = 4294967296;			//maximum time of the 826 on-board clock in microseconds
	uint TimeStampNew;					//init a local timestamp to compare with

	S826_TimestampRead(board, &TimeStampNew);	//get a new time stamp for the comparison
	if (TimeStampOld > TimeStampNew){		//check for time overflow
		*time = Tmax - TimeStampOld + TimeStampNew;		//difference in time calculation if there is a clock overflow
	}
	else{
		*time = (TimeStampNew - TimeStampOld); // / 1000;		//difference in time calculation

	}

}

int main(void){
	uint brd = 0;															// S826 board number set by dip switches on the board
	uint chan = 0;															// counter channel number
	uint ctrmode = 0x0000800A;												// counter mode
	uint PLcount = 0;														// Pre-Load count for resetting the counter after an event
	uint preload0reg = 0;													// Value to place in the preload0 register in pre-load
	uint TimestampOld = 0;
	uint Time = 0;
	uint diochan1 = 23;					//setting the channel for digital out
	uint diomask1[] = DIOMASK(DIO(diochan1));		//masking the chnannel of our digital out
	int Count = 0;


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
		S826_CounterStateWrite(brd, chan, 1);							// set the counter state to 1
	


		S826_TimestampRead(brd, &TimestampOld);		//Get timestamp for the start of the loop
		S826_DioOutputWrite(brd, diomask1, 1);		//write DIO pin 0 to high to simulate a G1/G0 signal 
		while (Time < 3000000){							// loop for 1 second
			LinearVelocity(brd, chan, TimestampOld);	// call the linear velocity function
			ElapsedTime(brd, TimestampOld, &Time);		//check the elapsed time

		}
		S826_DioOutputWrite(brd, diomask1, 0);		//turn off simulated G1/G0 signal

		Time = 0;			//reset time to 0
		Count++;
		printf("# of cycles: %d\n", Count);
		Sleep(1000);		//short sleep cycle for testing
	}
	system("pause");
	return 0;

}