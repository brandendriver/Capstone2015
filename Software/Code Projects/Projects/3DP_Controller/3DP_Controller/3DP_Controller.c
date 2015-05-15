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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                    Functions for use in Main                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//    The following functions are credited to Sensoray and their demo program with minor modification.   //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Counter mode: count down at 1MHz, preload when 0 reached, use both preload registers, assert ExtOut when Preload1 is active
#define PWM_MODE  (S826_CM_K_1MHZ | S826_CM_UD_REVERSE | S826_CM_PX_ZERO | S826_CM_PX_START | S826_CM_BP_BOTH | S826_CM_OM_PRELOAD)

// Helpful macros for DIOs
#define DIO(C)                  ((uint64)1 << (C))                          // convert dio channel number to uint64 bit mask
#define DIOMASK(N)              {(uint)(N) & 0xFFFFFF, (uint)((N) >> 24)}   // convert uint64 bit mask to uint[2] array
#define DIOSTATE(STATES,CHAN)   ((STATES[CHAN / 24] >> (CHAN % 24)) & 1)    // extract dio channel's boolean state from uint[2] array


#define X826(FUNC)   if ((errcode = FUNC) != S826_ERR_OK) { printf("\nERROR: %d\n", errcode); return errcode;}

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

	for (slot = 0; slot < 8; slot++)                           // sum the 8 samples from each analog input 
		sum[slot >> 3] += (short)(adcbuf[slot] & 0xFFFF);		// masking off sample meta-data

	*AdcVal = ((double)(sum[0] * 10) / (8 * 32768));
	//printf("CH0=%3.3fV\n", (double)(sum[0] * 10) / (8 * 32768));     // compute average values, report measured voltages for debugging
	//		}


	X826(S826_AdcEnableWrite(board, 0));  // halt adc conversions
	return errcode;
}

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//              Functions below this point were written by the 2015 capstone group.                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

static int WireSpeed(uint brd){

	float rad = .975;															//radius of wheel attached to the encoder in inches
	int tmax = 1000000;											// max interval time in microseconds, this will wait until event edge event is detected
	uint val = 0;
	uint timestamp, reason;
	uint chan = 0;

	S826_CounterSnapshotRead(brd, chan, &val, &timestamp, &reason, tmax); //Obtain a snapshot of what the counts buffer has after tmax time

	double pi = 3.1415;
	float numerator = val * pi * rad;									// calculate the numerator value for linear velocity
	int fullrotation = 500;												// nuber of counts in a full rotation of the encoder
	float lin = numerator / fullrotation;									// calculates the linear velocity of wirefeed in inches/second
	printf("Wire Feed = %.2f in/sec\t\n", lin);									// display the current linear velocity, should update every second.
}


static int CounterFrequencyIni(uint brd){									//Function for initializing counter in frequency measurement mode
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

static int DIOInitialization(){
	//use to initalize DIOs globally in order to prevent accidental overwriting
}

//////////////////////////////////////////////////////////////////
/*
//               ADJUST TIMING VALUES BEFORE USE                //


static int DropletSpacing(uint board, uint RunTime, uint *TimeStampOld, uint *Peaks, double *AdcValOld, double *AdcValNew){
	uint diochan1 = 23;					//setting the channel for digital out
	uint diomask1[] = DIOMASK(DIO(diochan1));		//masking the channel of our digital out
	uint time = 0;						// this is the elapsed time of a sampling cycle

	CounterFrequencyIni(board);		//initialize the counter in frequency measurement mode

	while (RunTime < 300000000){							//check if runtime is less than an arbitrary runtime in microseconds
		//WireSpeed(brd);									//measure and display wirespeed
		uint TimeStampNew = 0;
		S826_TimestampRead(board, &TimeStampOld);				//gets time stamp at beginning of program
		S826_DioOutputWrite(board, diomask1, 1);		//set the DIO pin to high to see the start of a sample on Oscilloscope
		while (time <= 1000000){

			AnalogVal_Chan0(board, &AdcValNew);		//get a new analog value from the onboard ADC, returns value in volts

			float Threshold = 6.5;						//the threshold for considering it a "peak" is 1v

			if (*AdcValNew > Threshold){			//check if the new value is larger than the threshold
				do{
					AnalogVal_Chan0(board, &AdcValNew);		//continue getting new analog values as long as they're greater than the threshold

				} while (*AdcValNew > Threshold);
				Peaks++;			// increment the peaks count, because all previously seen values were one single peak
			}

			ElapsedTime(board, TimeStampOld, &time);		//check the elapsed time against the original timestamp
			//printf("%u\n", time);

		}
		WireSpeed(board);
		S826_DioOutputWrite(board, diomask1, 0);		//set the DIO pin to low to see the end of a sample on the Oscilloscope
		Sleep(1000);

		if (Peaks != 0){		//check if the system saw a peak
			printf("Average Peak Time %u\t", time / *Peaks);		//calculate and print the average peak time
			printf("Number of Peaks %u\t\n", Peaks);		//print the number of peaks seen
		}
		else{
			printf("No peaks detected\n");
		}
		RunTime = RunTime + time;		//calculate the total runtime
		time = 0;		//reset the sample time
		Peaks = 0;		//reset the number of peaks seen
	}

}

//////////////////////////////////////////////////////////////////
*/

/* finish testing degrees to pulse rotations before uncommenting
static int SetWireSpeed(uint board, int WireSetting){
	uint counter = 5;
	uint TimeStampOld;
	int direction = 0;

	if (WireSetting < 0){
	direction = 0;
	}

	S826_DioOutputSourceWrite(board, diomask1);  // route counter's ExtOut signal to dio pin


		PwmGeneratorStart(board, counter, 500, 500);  // start pwm generator with 300ms/700ms on/off times
		S826_TimestampRead(board, &TimeStampOld);		// get initial timestamp for the start of the loop

		ElapsedTime(board, TimeStampOld, &time);		//Check the elapsed time in the loop
		while (time <= 800000){		//begin loop as long as elapsed time is less than 800ms

			ElapsedTime(board, TimeStampOld, &time);		//check the elapsed time once more
			printf("time is: %u\n", time);		//print the elapsed time
		}
		PwmGeneratorStop(board, counter);	//stop the pwm generator
		Sleep(1000);	//short sleep cycle as a delay
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                         MAIN FUNCTION                                                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	uint brd = 0;                        // change this if you want to use other than board number 0
	uint TimeStampOld;					//this is for calculating the elapsed time of a sampling cycle
	uint time = 0;						// this is the elapsed time of a sampling cycle
	uint RunTime = 0;					// Calculated total runtime of the below loop
	uint Peaks = 0;						// number of peaks counted in the loop
	uint diochan1 = 23;					//setting the channel for digital out
	uint diomask1[] = DIOMASK(DIO(diochan1));		//masking the channel of our digital out
	double AdcValOld = 0;		//first measured value from ADC. Measured off of the current sensor.
	double AdcValNew = 0;		//second measured value from ADC. Compared against AdcValOld to check for peaks



	int errcode = S826_ERR_OK;
	int boardflags = S826_SystemOpen();        // open 826 driver and find all 826 boards

	if (boardflags < 0)
		errcode = boardflags;                       // problem during open
	else if ((boardflags & (1 << brd)) == 0)
		printf("TARGET BOARD NOT FOUND\n");         // driver didn't find board you want to use

	//////////////////////////////////////////////////////////////////////////////////
	//                            Main Loop Starts Here                             //
	//////////////////////////////////////////////////////////////////////////////////

	else
	{
	// Prompt user for CNC speed

		// Verify CNC speed is correct

	// Prompt user for Welder Current setting

		// Verify Current setting is correct

	// Home wire speed setting on welder via stepper motor

	// Set wire speed, which is detemined by both CNC speed and current settings

	// Do nothing while G0 = 1 and G1 = 0

	// When G0 = 0 and G1 = 1, stop CNC movement

	// Check that baseplate temperature is above threshold

		// Baseplate temperature is below threshold

			// Run Torch Routine
		
		// Baseplate temperature is above threshold

	// Get TimeStamp

		// Use relay to start welder

		// Read current spikes

			// If current spikes = 0, ERROR. Stop welder and CNC. Exit Program

		// Start CNC movement

		// Check if G1 = 1 and G0 = 0. If not so, wait 
		
			// Check droplet spacing

				// If spacing is greater than +/- 20%, ERROR. Stop welder and CNC. Exit Program

				// If spacing is greater than +/- 5%, adjust wire speed control

		// Read TimeStamp
		
				// If loop time is less than 20% of Duty Cycle, check status of G0 and G1 and continue with weld
		
				// If loop time is greater than 20% of Duty Cycle, stop welder and CNC. Wait 80% of Duty Cycle

	
			// Check Temperature

				// If temp is above threshold, continue 

				// If temp is below threshold, stop CNC and Welder

					// Torch Routine

	// End Program
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
