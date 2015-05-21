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



//for use reading current sensor
/*
static int AnalogVal_Chan0(uint brd, double *AdcVal)
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
		X826(S826_AdcSlotConfigWrite(brd, slot, attr[slot].chan, attr[slot].tsettle, S826_ADC_GAIN_1));

	// Configure adc system and start it running.
	X826(S826_AdcSlotlistWrite(brd, 0xFFFF, S826_BITWRITE));  // enable all timeslots
	X826(S826_AdcTrigModeWrite(brd, 0));  // select continuous (untriggered) mode
	X826(S826_AdcEnableWrite(brd, 1));  // enable conversions

	// Fetch and display oversampled data.


	int sum[] = { 0, 0 };                                         // zero the adc sample accumulators, one per analog input channel
	slotlist = 0xFFFF;
	X826(S826_AdcRead(brd, adcbuf, NULL, &slotlist, 1000)); // read adc data from 16 slots

	for (slot = 0; slot < 8; slot++)                           // sum the 8 samples from each analog input 
		sum[slot >> 3] += (short)(adcbuf[slot] & 0xFFFF);		// masking off sample meta-data

	*AdcVal = ((double)(sum[0] * 10) / (8 * 32768));

	X826(S826_AdcEnableWrite(brd, 0));  // halt adc conversions
	return errcode;
}
*/

//for use reading temperature sensor
static int AnalogVal(uint brd, double *AdcVal, uint chan)
{
	// Adc timeslot attributes.
	// In each adc burst, convert analog channel 0 eight times then channel 1 eight times. Allow each channel to settle for 50 microseconds
	// after switching (before first conversion); subsequent conversions on the same channel don't need additional settling time.


	struct SLOTATTR {
		uint chan;      // analog input channel
		uint tsettle;   // settling time in microseconds
	}
	attr[8] = {                                                          // during each adc burst:
		{ chan, 50 }, { chan, 0 }, { chan, 0 }, { chan, 0 }, { chan, 0 }, { chan, 0 }, { chan, 0 }, { chan, 0 } };    //   switch to channel 0, wait 50us, digitize eight times   //  

	int i;
	int slot;
	uint slotlist;
	int adcbuf[16];
	int errcode;


	// Program attributes for all timeslots.
	for (slot = 0; slot < 8; slot++)
		X826(S826_AdcSlotConfigWrite(brd, slot, attr[slot].chan, attr[slot].tsettle, S826_ADC_GAIN_1)); //configure ADC to read -10V to +10V range

	// Configure adc system and start it running.
	X826(S826_AdcSlotlistWrite(brd, 0xFFFF, S826_BITWRITE));  // enable all timeslots
	X826(S826_AdcTrigModeWrite(brd, 0));  // select continuous (untriggered) mode
	X826(S826_AdcEnableWrite(brd, 1));  // enable conversions

	// Fetch and display oversampled data.

	int sum[] = { 0, 0 };                                         // zero the adc sample accumulators, one per analog input channel
	slotlist = 0xFFFF;
	X826(S826_AdcRead(brd, adcbuf, NULL, &slotlist, 1000)); // read adc data from 16 slots

	for (slot = 0; slot < 8; slot++)                           // sum the 8 samples from each analog input 
		sum[slot >> 3] += (short)(adcbuf[slot] & 0xFFFF);		// masking off sample meta-data

	*AdcVal = ((double)(sum[0] * 10) / (8 * 32768));		//calculate average of burst samples


	X826(S826_AdcEnableWrite(brd, 0));  // halt adc conversions
	return errcode;
}

// Configure a counter channel to operate as a pwm generator and start it running.
static int PwmGeneratorStart(uint brd, uint ontime, uint offtime)
{
	uint counter = 5; 
	S826_CounterStateWrite(brd, counter, 0);  // halt counter channel if it's running
	S826_CounterModeWrite(brd, counter, PWM_MODE);  // configure counter as pwm generator
	S826_CounterSnapshotConfigWrite(brd, counter, 0, S826_BITWRITE);  // don't need counter snapshots -- we're just outputting pwm signal
	S826_CounterPreloadWrite(brd, counter, 0, offtime);  // program pwm on-time in microseconds
	S826_CounterPreloadWrite(brd, counter, 1, ontime);  // program pwm off-time in microseconds
	S826_CounterStateWrite(brd, counter, 1);  // start pwm generator
	return 0;
}

// Halt channel operating as pwm generator.
static int PwmGeneratorStop(uint brd)
{
	uint counter = 5;
	return S826_CounterStateWrite(brd, counter, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//              Functions below this point were written by the 2015 capstone group.                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define G1G0INPUTMASK 0x000C0000
#define G1G0OUTPUTMASK 0x00030000
#define G1ACTIVE 0x40000
#define G0ACTIVE 0x80000
#define DIRECTIONMASK 0x00400000
#define WIRESPEEDHOMEMASK 0x00100000
#define PULSEMASK 0x00200000
#define MAXTURN 4222180
#define HOMEOFFSET 1.1
#define PWMTIMECONSTANT 88888							// Value determined by [(PWM on time)/(degree of rotation)]*[(degree of wire speed adjustment on welder)/(wire feed rate)] = microsecond/(inch per second)
#define STOPCNCWELDMASK 0x00830000
#define STARTCNCMASK 0x00800000
#define TEMPERATURECHAN 1
#define CURRENTCHAN 0
#define TEMPSENSORCONSTANT 140					// Value determined by [(temp range)/(Vin range)] = degrees C / Voltage input
#define HIGHTEMPTHRESHOLD  1200
#define LOWTEMPTHRESHOLD 800
#define SPIKECHECK 0
#define SPIKEMEASURE 1
#define MINCNCSETTING 3.0
#define MAXCNCSETTING 6.7
#define WELDDUTYCYCLE 120000000

extern float AvgDropletError[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int WatchDogEnable(uint brd){

	int rc;
	// enable writing to safe mode registers
	rc = S826_SafeWrenWrite(brd, 0x02);
	if (rc != 0) {
		printf("failed to enable wren for watchdog\n");
		return rc;
	}
}

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int ElapsedTime(uint brd, uint TimeStampOld, uint *time){		//takes board number, a time stamp from main, and a pointer to a time value in main, checks for time overflow and returns an elapsed time.
	uint64_t Tmax = 4294967296;			//maximum time of the 826 on-board clock in microseconds
	uint TimeStampNew;					//init a local timestamp to compare with

	S826_TimestampRead(brd, &TimeStampNew);	//get a new time stamp for the comparison
	if (TimeStampOld > TimeStampNew){		//check for time overflow
		*time = Tmax - TimeStampOld + TimeStampNew;		//difference in time calculation if there is a clock overflow
	}
	else{
		*time = (TimeStampNew - TimeStampOld);		//difference in time calculation

	}

}

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
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

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
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

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes board number, and pointers to two different dio state arrays, returns masked values of both arrays for manipulation in main.
*/
static int GetAndUpdateG0G1(uint brd, uint *diostatesA, uint *diostatesB){
	uint diostatesB_Mask[2] = { G1G0OUTPUTMASK, 0 };
	uint diostatesComp[2];

	S826_DioInputRead(brd, diostatesA);	//read the digital input states
	diostatesA[0] &= G1G0INPUTMASK;  //masking to the first two bits of the 3rd byte (channels 19 & 18)

	diostatesComp[0] = (~diostatesA[0] >> 2);		//bitshift right by two bits

	S826_DioOutputRead(brd, diostatesB);		//read the digital out states
	diostatesB[0] &= G1G0OUTPUTMASK;		//mask off the last two bits of the 3rd byte (channels 17 & 16)

	if ((diostatesB[0] != diostatesComp[0])){	//check if SHIFTED input dio is the same as current output dio
		S826_DioOutputWrite(brd, diostatesB_Mask, 1);
		S826_DioOutputWrite(brd, diostatesComp, 2);		//if it's not, write the SHIFTED input to output

	}
	if (diostatesA[0] == 0xC0000); {	//check if G0G1 are in unrecognized states, i.e. 11
		//Estop();		//Stop CNC machine AND welder
		int PrintError = 0;		//Set the error code for unrecognized states
		//ErrorDecode(PrintError);		//decode the error and halt program
	}

}

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int GetInputG0G1(uint brd, uint *diostatesA, int PrintError){

	S826_DioInputRead(brd, diostatesA);	//read the digital input states
	diostatesA[0] &= G1G0INPUTMASK;  //masking to the first two bits of the 3rd byte (channels 19 & 18)

	if (diostatesA[0] == 0xC0000); {	//check if G0G1 are in unrecognized states, i.e. 11
		Estop();		//Stop CNC machine AND welder
		PrintError = 0;		//Set the error code for unrecognized states
		ErrorDecode(PrintError);		//decode the error and halt program
	}

}

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int HaltCncAndWeld(uint brd){
	int HaltAll[2] = {STOPCNCWELDMASK, 0};

	S826_DioOutputWrite(brd, HaltAll, 1);

}

/* 
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int ErrorDecode(int error){

	switch (error){

	case 0:					//error code 0 is when G0 and G1 are both either high or both low
		printf("G1 & G0 combination is bad, i.e. CNC is saying G1 and G0 are both on."); break;
	case 1:					//error code 1 is when we see no current spikes at all in G1 (deposition mode)
		printf("No current spikes seen, check welder settings and/or wire."); break;
	case 2:					//error code 2 is when we see an error in droplet spacing greater than +/-20%
		printf("Droplet spacing error is greater than +/-20% , check welder settings."); break;
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

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int DropletSpacing(uint brd, double *AverageTime, uint RunType){
	uint diochan1 = 23;					//setting the channel for digital out
	uint diomask1[] = DIOMASK(DIO(diochan1));		//masking the channel of our digital out
	uint time = 0;						// this is the elapsed time of a sampling cycle
	double AdcValOld = 0;		//first measured value from ADC. Measured off of the current sensor.
	double AdcValNew = 0;		//second measured value from ADC. Compared against AdcValOld to check for peaks
	uint Peaks = 0;
	uint TimeStampOld = 0;
	uint SampleTime;
	float SpikeThreshold = 6.5;						//the threshold for considering it a "peak" is 1v
	
	//This section is for calculating the average time between peaks
	if (RunType == 1){
		SampleTime = 500000;
		S826_TimestampRead(brd, &TimeStampOld);				//gets time stamp at beginning of program
		while (time <= SampleTime){
			AnalogVal(brd, &AdcValNew,CURRENTCHAN);		//get a new analog value from the onboard ADC, returns value in volts
			if (AdcValNew > SpikeThreshold){			//check if the new value is larger than the threshold
				do{
					AnalogVal(brd, &AdcValNew, CURRENTCHAN);		//continue getting new analog values as long as they're greater than the threshold
				} while (AdcValNew > SpikeThreshold);
				Peaks++;			// increment the peaks count, because all previously seen values were one single peak
			}
			ElapsedTime(brd, TimeStampOld, &time);		//check the elapsed time against the original timestamp
		}
		*AverageTime = time / Peaks;
	}


	//This section is only for detecting if welder is welding
	else{
		SampleTime = 40000;
		S826_TimestampRead(brd, &TimeStampOld);				//gets time stamp at beginning of program
		while (time <= SampleTime){
			AnalogVal(brd, &AdcValNew, CURRENTCHAN);		//get a new analog value from the onboard ADC, returns value in volts
			if (AdcValNew > SpikeThreshold){			//check if the new value is larger than the threshold
				do{
					AnalogVal(brd, &AdcValNew, CURRENTCHAN);		//continue getting new analog values as long as they're greater than the threshold
				} while (AdcValNew > SpikeThreshold);
				Peaks++;			// increment the peaks count, because all previously seen values were one single peak
			}

			ElapsedTime(brd, TimeStampOld, &time);		//check the elapsed time against the original timestamp
			if (Peaks > 2){
				break;
			}
		}
	}

		
		if (Peaks == 0){
			HaltCncAndWeld(brd);
			uint errorcode = 1;
			ErrorDecode(errorcode);
		}

}

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
//finish testing degrees to pulse rotations before uncommenting
static int SetWireSpeed(uint brd, float setting){
	uint time = 0;
	uint TimeStampOld = 0;
	uint direction[2] = { DIRECTIONMASK, 0 };

	if (setting > 0){
		S826_DioOutputWrite(brd, direction, 1);

	}
	else if (setting < 0){
		S826_DioOutputWrite(brd, direction, 1);
		S826_DioOutputWrite(brd, direction, 2);
	}
	setting = setting - HOMEOFFSET;
	float TimeOn = PWMTIMECONSTANT * setting;

	PwmGeneratorStart(brd, 500, 500);
	S826_TimestampRead(brd, &TimeStampOld);		// get initial timestamp for the start of the loop

	ElapsedTime(brd, TimeStampOld, &time);		//Check the elapsed time in the loop
	while (time <= TimeOn){		//begin loop as long as elapsed time is less than 800ms

		ElapsedTime(brd, TimeStampOld, &time);		//check the elapsed time once more
		//printf("time is: %u\n", time);		//print the elapsed time
	}
	PwmGeneratorStop(brd);	//stop the pwm generator
}

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int HomeWireSpeed(uint brd){
	uint counter = 5;
	uint direction[2] = { DIRECTIONMASK, 0 };
	uint PulsePin[2] = { PULSEMASK, 0 };
	uint pinstate[2] = { WIRESPEEDHOMEMASK, 0 };

	S826_DioOutputWrite(brd, pinstate, 1);

	S826_DioInputRead(brd, pinstate);
	pinstate[0] &= WIRESPEEDHOMEMASK;
	S826_DioOutputSourceWrite(brd, PulsePin);

	if (pinstate[0] != 0x100000){
		S826_DioOutputWrite(brd, direction, 1);
		S826_DioOutputWrite(brd, direction, 2);
		PwmGeneratorStart(brd, counter, 5000, 5000);
		while (pinstate[0] != 0x100000){
			S826_DioInputRead(brd, pinstate);
			pinstate[0] &= WIRESPEEDHOMEMASK;
		}
		PwmGeneratorStop(brd, counter);
	}

	
}

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int UserPrompt(float *WireSpeedSetting, float *NominalDropletTime){

	float CNCSpeed = 0.0;
	float CNCRelatedToWireSpeed[10] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
	float NominalDroplets[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int a = 0;

	printf("Please input your CNC Speed, Note speed must be between 3 in/s and 6.7 in/s:\n ");
	scanf_s("%f", &CNCSpeed);
	while (CNCSpeed < MINCNCSETTING || CNCSpeed > MAXCNCSETTING){
		printf("Error. The CNC speed you have entered is outside the valid range.\n");
		Sleep(1000);
		printf("Please input your CNC Speed, Note speed must be between 3 in/s and 6.7 in/s:\n ");
		scanf_s("%f", &CNCSpeed);

	}
	printf("CNC speed is valid, setting up the system now.\n");
	a = (int)((CNCSpeed * 10) - 30);

	*WireSpeedSetting = CNCRelatedToWireSpeed[a];
	*NominalDropletTime = NominalDroplets[a];
}

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int GetTemp(uint brd, float *Temp){
	double ADCTemp;

	AnalogVal(brd, &ADCTemp, TEMPERATURECHAN);

	*Temp = ADCTemp * TEMPSENSORCONSTANT;
	*Temp = *Temp + 800;
}

/*
Function Name:
Function Inputs:
Function Outputs/Updates:
Function Description: takes an error code of 0,1,2 and prints appropriate error statement before exiting the program
*/
static int ErrorCalculation(uint brd, float DropletTime, float NominalDropletTime, float *DropletError){
	int i;
	float AvgSum = 0;

	for (i = 0; i < sizeof(AvgDropletError) -1; i++){
		AvgSum = AvgSum + AvgDropletError[i];
		AvgDropletError[i] = AvgDropletError[i + 1];
	}
	AvgDropletError[i] = DropletTime;

	*DropletError = (NominalDropletTime - DropletTime) / NominalDropletTime;


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                         MAIN FUNCTION                                                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	uint brd = 0;                        // change this if you want to use other than board number 0
	uint DutyCycleTimeStamp;
	uint time = 0;						// this is the elapsed time of a sampling cycle
	uint RunTime = 0;					// Calculated total runtime of the below loop
	uint Peaks = 0;						// number of peaks counted in the loop
	uint diochan1 = 23;					//setting the channel for digital out
	uint diomask1[] = DIOMASK(DIO(diochan1));		//masking the channel of our digital out
	uint InputStates[2] = { 0, 0 };
	uint OutputStates[2];
	uint Direction;

	float Temp = 0;

	float DropletTime = 0.0;
	float InitialWireSpeedSetting = 0.0;
	float NominalDropletTime = 0.0;
	float DropletError = 0.0;
	float AdjustedWireSpeed = 0;

	int loop = 0;
	int PrintError = 0;

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


	WatchDogEnable(brd);

		// Prompt user for CNC speed and verify that it is a correct speed value.
	UserPrompt(&InitialWireSpeedSetting, &NominalDropletTime);

		// Home wire speed setting on welder via stepper motor
	HomeWireSpeed(brd);

		// Set wire speed, which is detemined by both CNC speed and current settings
	SetWireSpeed(brd, InitialWireSpeedSetting, Direction);


		// Do nothing while G0 = 1 and G1 = 0
		
	while (InputStates[0] != G1ACTIVE){
			GetAndUpdateG0G1(brd, &InputStates, &OutputStates);
			
	}

	InitialPosition:
	// When G0 = 0 and G1 = 1, stop CNC movement
	HaltCncAndWeld(brd);
	// Check that baseplate temperature is above threshold
	GetTemp(brd, &Temp);
	while (Temp < LOWTEMPTHRESHOLD || Temp > HIGHTEMPTHRESHOLD){
		////////////////////////////////////////////////////////
		// Run Torch Routine: This is a manual process for now//
		////////////////////////////////////////////////////////
		GetTemp(brd, &Temp);

	}
	// Get TimeStamp
	S826_TimestampRead(brd, &DutyCycleTimeStamp);
		// Use relay to start welder
	GetAndUpdateG0G1(brd, NULL, NULL);

		// Read current spikes
		// If current spikes = 0, ERROR. Stop welder and CNC. Exit Program
	DropletSpacing(brd, NULL, SPIKECHECK);

		//Check Temperature again
	GetTemp(brd, &Temp);
	if (Temp < LOWTEMPTHRESHOLD || Temp > HIGHTEMPTHRESHOLD){
		goto InitialPosition;
	}
		// Start CNC movement
	S826_DioOutputWrite(brd, STARTCNCMASK, 1);
	S826_DioOutputWrite(brd, STARTCNCMASK, 2);

	GetTemp(brd, &Temp);
		// Check if G1 = 1 and G0 = 0. If not so, wait 
	G1Checking:
	do {
		GetAndUpdateG0G1(brd, &InputStates, &OutputStates);
	} while (InputStates[0] != G1ACTIVE);

	while (InputStates[0] == G1ACTIVE){

		// Check droplet spacing
		DropletSpacing(brd, &DropletTime, SPIKEMEASURE);

		///////////////		// If spacing is greater than +/- 20%, ERROR. Stop welder and CNC. Exit Program		///////////////	
		ErrorCalculation(brd, DropletTime, NominalDropletTime, &DropletError);
		if (loop >= sizeof(AvgDropletError)){
			if (DropletError >= 0.2 || DropletError <= -0.2){
				HaltCncAndWeld(brd);
			}
			///////////////		// If spacing is greater than +/- 5%, adjust wire speed control		///////////////	
			else if (DropletError >= 0.05 || DropletError <= -0.05){
				AdjustedWireSpeed = InitialWireSpeedSetting * DropletError;  //still need to test if this works for proportional control
				SetWireSpeed(brd, AdjustedWireSpeed);
			}
		}

		// Read TimeStamp
		ElapsedTime(brd, DutyCycleTimeStamp, &time);

		// If loop time is less than 20% of Duty Cycle, check status of G0 and G1 and continue with weld
		// If loop time is greater than 20% of Duty Cycle, stop welder and CNC. Wait 80% of Duty Cycle
		if (time > WELDDUTYCYCLE){
			HaltCncAndWeld(brd);
			time = 0;
			S826_TimestampRead(brd, &DutyCycleTimeStamp);
			printf("Welder has been running for longer than its duty cycle, waiting for cooldown now.\n");
			while (time < 480000000){
				ElapsedTime(brd, DutyCycleTimeStamp, &time);
			}
			printf("Welder has cooled down, checking temperature of weld now before proceeding with print.\n");
			goto InitialPosition;
		}

		GetAndUpdateG0G1(brd, &InputStates, &OutputStates);
		if (InputStates[0] != G1ACTIVE){
			goto G1Checking;
		}
		loop++;
	}

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

