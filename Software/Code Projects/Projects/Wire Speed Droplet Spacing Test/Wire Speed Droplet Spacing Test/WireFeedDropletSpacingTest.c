#ifndef _LINUX
#include <windows.h>
#include <conio.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#ifndef _LINUX
#include "826api.h"
#else
#include "826api.h"
#endif



#define X826(FUNC)   if ((errcode = FUNC) != S826_ERR_OK) { printf("\nERROR: %d\n", errcode); return errcode;}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////
static int AnalogVal_Chan0(uint board, double *AdcVal)
{
	// Adc timeslot attributes.
	// In each adc burst, convert analog channel 0 eight times then channel 1 eight times. Allow each channel to settle for 50 microseconds
	// after switching (before first conversion); subsequent conversions on the same channel don't need additional settling time.
	struct SLOTATTR {
		uint chan;      // analog input channel
		uint tsettle;   // settling time in microseconds
	} 
	attr[8] = {                                                          // during each adc burst:
		{ 0, 50 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };    //   switch to channel 0, wait 50us, digitize eight times   //  

	int i;
	int slot;
	uint slotlist;
	int adcbuf[16];
	int errcode;


	// Program attributes for all timeslots.
	for (slot = 0; slot < 8; slot++)
		X826(S826_AdcSlotConfigWrite(board, slot, attr[slot].chan, attr[slot].tsettle, S826_ADC_GAIN_1));

	// Configure adc system and start it running.
	X826(S826_AdcSlotlistWrite(board, 0xFFFF, S826_BITWRITE));  // enable all timeslots
	X826(S826_AdcTrigModeWrite(board, 0));  // select continuous (untriggered) mode
	X826(S826_AdcEnableWrite(board, 1));  // enable conversions

	// Fetch and display oversampled data.

	//	for (i = 0; i < 12; i++)                                        // repeat a few times:
		//{
			int sum[] = { 0, 0 };                                         // zero the adc sample accumulators, one per analog input channel
			slotlist = 0xFFFF;
			X826(S826_AdcRead(board, adcbuf, NULL, &slotlist, 1000)); // read adc data from 16 slots

			for (slot = 0; slot < 16; slot++)                           // sum the 8 samples from each analog input 
				sum[slot >> 3] += (short)(adcbuf[slot] & 0xFFFF);		// masking off sample meta-data

			*AdcVal = ((double)(sum[0] * 10) / (8 * 32768));
			//printf("CH0=%3.3fV\n", (double)(sum[0] * 10) / (8 * 32768));     // compute average values, report measured voltages
//		}
	

	X826(S826_AdcEnableWrite(board, 0));  // halt adc conversions
	return errcode;
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
	float numerator = val * 2 * pi * rad;									// calculate the numerator value for linear velocity
	int fullrotation = 5000;												// nuber of counts in a full rotation of the encoder
	float lin = numerator / fullrotation;									// calculates the linear velocity of wirefeed in inches/second
	printf("Wire Feed = %.2f in/sec\t" , lin);									// display the current linear velocity, should update every second.
}

static int AveragePeakTime(uint Peaks, uint SampleTime) {
	float PeakAvg;

	PeakAvg = SampleTime / Peaks;
	printf("Average Peak Time %u\n", PeakAvg);


}

static int CounterIni(uint brd){											//Function for initializing counter in frequency measurement mode
	uint chan = 0;															// counter channel number
	uint ctrmode = 0x00008009;												// counter mode
	uint PLcount = 0;														// Pre-Load count for resetting the counter after an event
	uint preload0reg = 0;

	S826_CounterStateWrite(brd, chan, 0);								//Deactive counter
	S826_CounterModeWrite(brd, chan, ctrmode);						   // use the CounterModeWrite function to set the mode of the counter (in this case the mode is set to frequency measurement)
	S826_CounterSnapshotConfigWrite(brd, chan, 0x10, 0);			   // use the CounterSnapshotConfigureWrite to configure the snapshot collection on our counter
	S826_CounterPreloadWrite(brd, chan, preload0reg, PLcount);		   // use the CounterPreloadWrite function to pre-load 0 into the counts buffer.
	S826_CounterStateWrite(brd, chan, 1);								//Activate counter

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main function.

int main(void)
{
	uint brd = 0;                        // change this if you want to use other than board number 0
	uint TimeStampOld;
	uint time = 0;
	uint RunTime = 0;
	uint Peaks = 0;
	uint PeakAvg;
	double AdcValOld = 0;
	double AdcValNew = 0;


	
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
		

		AnalogVal_Chan0(brd, &AdcValOld);
		while(RunTime < 40000000){
			//WireSpeed(brd);									//measure and display wirespeed
			uint TimeStampNew = 0;
			S826_TimestampRead(brd, &TimeStampOld);				//gets time stamp at beginning of program

			while(time <= 8000000){
				AnalogVal_Chan0(brd, &AdcValNew);
				float Threshold = AdcValOld - .6;
				if (AdcValNew < Threshold){
					Peaks++;
				}

				AdcValOld = AdcValNew;
				ElapsedTime(brd, TimeStampOld, &time);
			}
			//AveragePeakTime(Peaks, &PeakAvg, T1);
			if (Peaks != 0){
				printf("Average Peak Time %u\t", time / Peaks);
				printf("Number of Peaks %u\n", Peaks);
			}
			else{
				printf("Error, no peaks detected\n");
			}
			RunTime = RunTime + time;
			time = 0;
			Peaks = 0;
		}
		
	

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

