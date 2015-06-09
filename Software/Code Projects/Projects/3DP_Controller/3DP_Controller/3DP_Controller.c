////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2015  3D Metal Printing Capstone									///
///																					///
/// This program is free software; you can redistribute it and/or					///
/// modify it under the terms of the GNU General Public License						///
/// as published by the Free Software Foundation; either version 2					///
/// of the License, or (at your option) any later version.							///
///																					///
/// This program is distributed in the hope that it will be useful,					///
/// but WITHOUT ANY WARRANTY; without even the implied warranty of					///
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					///
/// GNU General Public License for more details.									///
///																					///
/// You should have received a copy of the GNU General Public License				///
/// along with this program; if not, write to the Free Software						///
/// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.	///
////////////////////////////////////////////////////////////////////////////////////////

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
#define BRD 0



static int AnalogVal(float *AdcVal, uint chan)
{
	// Adc timeslot attributes.
	// In each adc burst, convert analog channel 0 eight times then channel 1 eight times. Allow each channel to settle for 50 microseconds
	// after switching (before first conversion); subsequent conversions on the same channel don't need additional settling time.


	struct SLOTATTR {
		uint chan;      // analog input channel
		uint tsettle;   // settling time in microseconds
	}
	attr[8] = {         // during each adc burst:
			{ chan, 50 },
			{ chan, 0 },
			{ chan, 0 },
			{ chan, 0 },
			{ chan, 0 },
			{ chan, 0 },
			{ chan, 0 },
			{ chan, 0 } };   //   switch to channel 0, wait 50us, digitize eight times 

	int i;
	int slot;
	uint slotlist;
	int adcbuf[16];
	int errcode;


	// Program attributes for all timeslots.
	for (slot = 0; slot < 8; slot++)
	{
		S826_AdcSlotConfigWrite(BRD, slot, attr[slot].chan, attr[slot].tsettle, S826_ADC_GAIN_1); //configure ADC to read -10V to +10V range
	}

	// Configure adc system and start it running.
	S826_AdcSlotlistWrite(BRD, 0xFFFF, S826_BITWRITE);  	// enable all timeslots
	S826_AdcTrigModeWrite(BRD, 0);  						// select continuous (untriggered) mode
	S826_AdcEnableWrite(BRD, 1);  							// enable conversions

	// Fetch and display oversampled data.

	int sum[] = { 0, 0 };                               	// zero the adc sample accumulators, one per analog input channel
	slotlist = 0xFFFF;
	S826_AdcRead(BRD, adcbuf, NULL, &slotlist, 1000); 		// read adc data from 16 slots

	for (slot = 0; slot < 8; slot++)                        // sum the 8 samples from each analog input
	{
		sum[slot >> 3] += (short)(adcbuf[slot] & 0xFFFF);	// masking off sample meta-data
	}

	*AdcVal = ((float)(sum[0] * 10) / (8 * 32768));		//calculate average of burst samples


	X826(S826_AdcEnableWrite(BRD, 0));  					// halt adc conversions
	return errcode;
}

// Configure a counter channel to operate as a pwm generator and start it running.
static int PwmGeneratorStart(uint ontime, uint offtime)
{
	uint counter = 5;
	S826_CounterStateWrite(BRD, counter, 0);  							// halt counter channel if it's running
	S826_CounterModeWrite(BRD, counter, PWM_MODE);  					// configure counter as pwm generator
	S826_CounterSnapshotConfigWrite(BRD, counter, 0, S826_BITWRITE); 	// don't need counter snapshots -- we're just outputting pwm signal
	S826_CounterPreloadWrite(BRD, counter, 0, offtime);  				// program pwm on-time in microseconds
	S826_CounterPreloadWrite(BRD, counter, 1, ontime);  				// program pwm off-time in microseconds
	S826_CounterStateWrite(BRD, counter, 1);  							// start pwm generator
	return 0;
}

// Halt channel operating as pwm generator.
static int PwmGeneratorStop()
{
	uint counter = 5;
	return S826_CounterStateWrite(BRD, counter, 0);
}

/*
Function Name: Watch Dog Enable
Inputs: Board number
Outputs/Updates: Updates: the watchdog's SafeWrenWrite function
Function Description: By updating the Watchdog's SafeWrenWrite function it enables and starts the watchdog timer so that PWM mode can be utilized.
*/
static int WatchDogEnable()
{
	int rc;
	// enable writing to safe mode registers
	rc = S826_SafeWrenWrite(BRD, 0x02);
	if (rc != 0)
	{
		printf("failed to enable wren for watchdog\n");
		return rc;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//              Functions below this point were written by the 2015 capstone group.                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define G1G0INPUTMASK 0x000C0000
#define G1G0OUTPUTMASK 0x00030000
#define G1OUTACTIVE 0x20000
#define G0OUTACTIVE 0x10000
#define G1INPUTACTIVE 0x40000
#define G0INPUTACTIVE 0x80000
#define G1G0INVALIDINPUT 0x00
#define DIRECTIONMASK 0x00400000
#define WIRESPEEDHOMEMASK 0x00100000
#define PULSEMASK 0x00200000
#define MAXTURN 4222180
#define PWMTIMECONSTANT 88888		/* Value determined by [(PWM on time)/(degree of rotation)]*[(degree of wire speed adjustment on welder)/(wire feed rate)] = microsecond/(inch per second) */
#define STOPCNCWELDMASK 0x00830000
#define STARTCNCMASK 0x00800000
#define TEMPERATURECHAN 1
#define CURRENTCHAN 0
#define TEMPSENSORCONSTANT 140			/* Value determined by [(temp range)/(Vin range)] = degrees C / Voltage input */
#define HIGHTEMPTHRESHOLD 1200
#define LOWTEMPTHRESHOLD 900
#define SPIKECHECK 0
#define SPIKEMEASURE 1
#define MINCNCSETTING 5.2f
#define MAXCNCSETTING 5.8f
#define WELDDUTYCYCLE 120000000
#define COUNTSPERREV 5000
#define NOMINALDROPLETTIME 9000


/*
Function Name: Elapsed Time
Function Inputs: Board number, a time stamp (taken before entering the function), pointer to a time variable
Function Outputs/Updates: Updates: the time variable,
Function Description: The function checks for the overflow of the 826's on-board time stamp generator and calculates the time elapsed between an old time stamp and the time the function is called. It stores the elapsed time in a time variable in microseconds.
*/

static void ElapsedTime(uint TimeStampOld, uint *time) 	//takes board number, a time stamp from main, and a pointer to a time value in main, checks for time overflow and returns an elapsed time.
{
	uint64_t Tmax = 4294967296;							// maximum time of the 826 on-board clock in microseconds
	uint TimeStampNew;									// initialize a local timestamp to compare with

	S826_TimestampRead(BRD, &TimeStampNew);				// get a new time stamp for the comparison
	if (TimeStampOld > TimeStampNew)					// check for time overflow
	{
		*time = Tmax - TimeStampOld + TimeStampNew;		// difference in time calculation if there is a clock overflow
	}
	else
	{
		*time = (TimeStampNew - TimeStampOld);			// difference in time calculation
	}
}

/*
Function Name: Wire Speed
Inputs: Board number
Outputs/Updates: Prints wire feed speed
Function Description: The function obtains a snapshot of the counts buffer from the 826, which is attached to the encoder on the wire feed system of the welder. It then calculates the linear velocity of the wire being fed, and prints it to the screen.
*/
static void WireSpeed(float *WireSpeed)
{
	float dia = 0.975f;		// radius of wheel attached to the encoder in inches
	int tmax = 1000000;		// max interval time in microseconds, this will wait until event edge event is detected
	uint val = 0;
	uint timestamp, reason;
	uint chan = 0;

	S826_CounterSnapshotRead(BRD, chan, &val, &timestamp, &reason, tmax); //Obtain a snapshot of what the counts buffer has after tmax time

	float pi = 3.1415f;
	float numerator = val * pi * dia;				// calculate the numerator value for linear velocity
	float denominator = COUNTSPERREV * 0.1f;		// SECOND NUMBER IN DENOMINATOR IS ENCODER SAMPLE TIME, IF SAMPLE TIME IS CHANGED IN CounterFrequencyIni FUNCTION, THIS NUMBER NEEDS TO REFLECT THAT CHANGE
	float lin = numerator / denominator;			// calculates the linear velocity of wirefeed in inches/second
	*WireSpeed = lin;
	printf("Wire Feed = %.2f in/sec\t\n", lin);		// display the current linear velocity, should update every second.
}

/*
Function Name: Counter Frequency Init
Inputs: Board number
Outputs/Updates: Updates:
Function Description: Initializes the channel 0 counter into frequency mode, for use with calculating wire speed.
*/
static void CounterFrequencyIni() //Function for initializing counter in frequency measurement mode
{
	uint chan = 0;														// counter channel number
	uint ctrmode = 0x0000800A;											// counter mode, IF SAMPLE TIME IS TO BE CHANGED, ALSO CHANGE DENOMINATOR IN WireSpeed FUNCTION. See register descriptions.
	uint PLcount = 0;													// Pre-Load count for resetting the counter after an event
	uint preload0reg = 0;

	S826_CounterStateWrite(BRD, chan, 0);								// Deactivate counter
	S826_CounterModeWrite(BRD, chan, ctrmode);						   	// use the CounterModeWrite function to set the mode of the counter (in this case the mode is set to frequency measurement)
	S826_CounterSnapshotConfigWrite(BRD, chan, 0x10, 0);			   	// use the CounterSnapshotConfigureWrite to configure the snapshot collection on our counter
	S826_CounterPreloadWrite(BRD, chan, preload0reg, PLcount);		   	// use the CounterPreloadWrite function to pre-load 0 into the counts buffer.
	S826_CounterStateWrite(BRD, chan, 1);								// Activate counter
}

/*
Function Name: Halt CNC and Weld
Inputs: Board number
Outputs/Updates: CNC start/stop pin, Welder switching Pin.
Function Description: Outputs a high signal on both the CNC start/stop pin and the Welder Switching pin (NOTE: This is done by forcing the OUTPUT G0/G1 to the state "00" causing the welder trigger to halt unconditionally).
*/
static void HaltCncAndWeld()
{
	uint HaltAll[2] = { STOPCNCWELDMASK, 0 };		// Array of two data values, used for stopping BOTH the CNC and Welder
	uint StartCNCMask[2] = { STARTCNCMASK, 0 };		// Array of two data values, used for starting the CNC


	S826_DioOutputWrite(BRD, HaltAll, 1);			// Stops the CNC and the Welder
	S826_DioOutputWrite(BRD, StartCNCMask, 2);		// Starts the CNC machine again
}

/*
Function Name: Error Decode
Inputs: Error Code variable
Outputs/Updates: None
Function Description: Decodes input error code and prints appropriate error description before halting the system and exiting the program.
*/
static void ErrorDecode(int error)
{
	switch (error)
	{
	case 0:				// error code 0 is when G0 and G1 are both either high or both low
		printf("G1 & G0 combination is bad, i.e. CNC is saying G1 and G0 are both on.\n");
		break;
	case 1:				// error code 1 is when we see no current spikes at all in G1 (deposition mode)
		printf("No current spikes seen, check welder settings and/or wire.\n");
		break;
	case 2:				// error code 2 is when we see an error in droplet spacing greater than +/-20%
		printf("Droplet spacing error is greater than +/-20% , check welder settings.\n");
		break;
	default:			// Default case, i.e. error is not 0, 1, or 2.
		printf("Unknown error");
		break;
	}
	
#ifndef _LINUX	
	printf("\nKeypress to exit ...\n\n");
	while (!_kbhit());						// wait for key press from user
	_getch();
#endif
	
	S826_SystemClose();						// end the 826's processes
	exit(0);								// end the program
}

/*
Function Name: Get and update G0/G1
Inputs: Board number, Pointer to an array of DIO Inputs, and Pointer to an array of DIO Outputs
Outputs/Updates: Updates: G1G0 output array, G1G0 input array
Function Description: Reads the current Input G0/G1 states and checks if the current Output G0/G1 states are the same, if not it updates them. The function also error handles to check if the function is in an unrecognised state of both pins high, i.e. 11 and handles the error appropriately.
*/
static void GetAndUpdateG0G1(uint *diostatesA, uint *diostatesB)
{
	uint diostatesB_Mask[2] = { G1G0OUTPUTMASK, 0 };
	uint diostatesComp[2];


	S826_DioInputRead(BRD, diostatesA);					// read the digital input states

	diostatesA[0] = ~diostatesA[0];
	diostatesA[0] &= G1G0INPUTMASK;  					// masking to the first two bits of the 3rd byte (channels 19 & 18)

	diostatesComp[0] = (diostatesA[0] >> 2);			// bitshift right by two bits

	S826_DioOutputRead(BRD, diostatesB);				// read the digital out states
	diostatesB[0] &= G1G0OUTPUTMASK;					// mask off the last two bits of the 3rd byte (channels 17 & 16)

	if ((diostatesB[0] != diostatesComp[0]))			// check if SHIFTED input dio is the same as current output dio
	{
		S826_DioOutputWrite(BRD, diostatesB_Mask, 1);
		S826_DioOutputWrite(BRD, diostatesComp, 2);		// if it's not, write the SHIFTED input to output

	}

	diostatesA[0] = ~diostatesA[0];
	diostatesA[0] &= G1G0INPUTMASK;						// masking to the first two bits of the 3rd byte (channels 19 & 18)
	
	if (diostatesA[0] == G1G0INVALIDINPUT)				// check if G0G1 are in unrecognised states, i.e. 11
	{
		HaltCncAndWeld();								// Stop CNC machine AND welder
		int PrintError = 0;								// Set the error code for unrecognised states
		ErrorDecode(PrintError);						// decode the error and halt program
	}
	
}

/*
Function Name: Get Input G0/G1
Inputs: Board number, Pointer to an array of DIO Inputs, an error code variable
Outputs/Updates: Updates: G0/G1 Input array
Function Description: Reads and updates the current G0/G1 input states. The function also error handles to check if the function is in an unrecognised state of both pins high, i.e. 11 and handles the error appropriately.
*/
static void GetInputG0G1(uint *diostatesA, int PrintError)
{
	S826_DioInputRead(BRD, diostatesA);		// read the digital input states
	diostatesA[0] &= G1G0INPUTMASK;  		// masking to the first two bits of the 3rd byte (channels 19 & 18)

	
	if (diostatesA[0] == G1G0INVALIDINPUT) 	// check if G0G1 are in unrecognised states, i.e. 11
	{
		HaltCncAndWeld();					// Stop CNC machine AND welder
		PrintError = 0;						// Set the error code for unrecognised states
		ErrorDecode(PrintError);			// decode the error and halt program
	}
	
}

/*
Function Name: Droplet Spacing
Inputs: Board number, Pointer to an array of average time samples, A variable for the type of run the code should be in
Outputs/Updates: Outputs: the spacing between "droplets" Updates: Average Time between droplets variable
Function Description: Using the input "run type" the function runs in one of two modes; Spike Check, and Spike Measurement
Spike Check Mode: runs and averages values read from the analog input for only 40 milliseconds to make sure the welder is on and the current sensor is correctly reading by updating a peaks count for values above the threshold for a peak.
Spike Measurement Mode: runs and averages values read from the analog input for 500 milliseconds to get a more accurate average of the number of peaks seen in the current.
*/
static void DropletSpacing(float *AverageTime, uint RunType)
{
	uint diochan1 = 23;										// setting the channel for digital out
	uint diomask1[] = DIOMASK(DIO(diochan1));				// masking the channel of our digital out
	uint time = 0;											// this is the elapsed time of a sampling cycle
	float AdcValOld = 0.0f;									// first measured value from ADC. Measured off of the current sensor.
	float AdcValNew = 0.0f;									// second measured value from ADC. Compared against AdcValOld to check for peaks
	uint Peaks = 0;
	uint TimeStampOld = 0;
	uint SampleTime;
	float SpikeThreshold = 6.5f;							// the threshold for considering it a "peak" is 1v

	// This section is for calculating the average time between peaks
	if (RunType == 1)
	{
		SampleTime = 500000;
		S826_TimestampRead(BRD, &TimeStampOld);				// gets time stamp at beginning of program
		while (time <= SampleTime)
		{
			AnalogVal(&AdcValNew, CURRENTCHAN);				// get a new analog value from the onboard ADC, returns value in volts
			if (AdcValNew > SpikeThreshold)					// check if the new value is larger than the threshold
			{
				do
				{
					AnalogVal(&AdcValNew, CURRENTCHAN);		// continue getting new analog values as long as they're greater than the threshold
				} while (AdcValNew > SpikeThreshold);
				Peaks++;									// increment the peaks count, because all previously seen values were one single peak
			}
			ElapsedTime(TimeStampOld, &time);				//check the elapsed time against the original timestamp
		}
		if (Peaks == 0)
		{
			HaltCncAndWeld();
			uint errorcode = 1;
			ErrorDecode(errorcode);
		}
		*AverageTime = time / Peaks;
	}
	else													// This section is only for detecting if welder is welding
	{
		SampleTime = 1000000;
		S826_TimestampRead(BRD, &TimeStampOld);				// gets time stamp at beginning of program
		while (Peaks < 3)
		{
			AnalogVal(&AdcValNew, CURRENTCHAN);				// get a new analog value from the onboard ADC, returns value in volts
			if (AdcValNew > SpikeThreshold)					// check if the new value is larger than the threshold
			{
				do
				{
					AnalogVal(&AdcValNew, CURRENTCHAN);		// continue getting new analog values as long as they're greater than the threshold
				} while (AdcValNew > SpikeThreshold);
				Peaks++;									// increment the peaks count, because all previously seen values were one single peak
			}

			ElapsedTime(TimeStampOld, &time);				// check the elapsed time against the original timestamp
			if (time > SampleTime)
			{
				break;
			}
			
		}
	}

	if (Peaks == 0)											// check if the number of peaks seen is 0
	{
		HaltCncAndWeld();									// stop the CNC and the Welder
		uint errorcode = 1;									// set the error code to 1
		ErrorDecode(errorcode);								// pass the error code the error decode function
	}
}

/*
Function Name: Set Wire Speed
Inputs: Board number, Wire Speed Setting
Outputs/Updates: Updates: Turns Wire Speed knob via stepper motor
Function Description: checks to see if the input setting is positive or negative and sets the direction the motor will turn appropriately. The function then uses the input setting value and calculates based on the homing offset how long the motor must turn to reach the requested wire speed setting. Finally it turns the motor for the appropriate amount of time.
*/
static void SetWireSpeed(float *setting)
{
	uint time = 0;
	uint TimeStampOld = 0;
	uint direction[2] = { DIRECTIONMASK, 0 };
	uint pinstate[2] = { WIRESPEEDHOMEMASK, 0 };

	S826_DioOutputWrite(BRD, pinstate, 1);		// Clear the states of the wire speed homing pins	

	if (*setting > 0)							// check if the setting is positive
	{
		S826_DioOutputWrite(BRD, direction, 1);	// set the movement direction accordingly 

	}
	else if (*setting < 0)						// check if the setting is negative
	{
		S826_DioOutputWrite(BRD, direction, 1);	// set the movement direction accordingly by clearing
		S826_DioOutputWrite(BRD, direction, 2);	// and writing
	}

	*setting = fabsf(*setting);					// take the absolute value of the setting (this is to account for negative settings)
	float TimeOn = PWMTIMECONSTANT * *setting;	// calculate the amount of time that the motor needs to turn on for in order to reach the new setting

	PwmGeneratorStart(500, 500);				// turn on the PWM generator for a 500 on / 500 off microsecond cycle.
	S826_TimestampRead(BRD, &TimeStampOld);		// get initial timestamp for the start of the loop

	ElapsedTime(TimeStampOld, &time);			// Check the elapsed time in the loop
	while (time <= TimeOn)						// begin loop as long as elapsed time is less than 800ms
	{

		ElapsedTime(TimeStampOld, &time);		// check the elapsed time once more
		//printf("time is: %u\n", time);		// print the elapsed time USED IN DEBUGGING
	}
	PwmGeneratorStop();							// stop the pwm generator
}

/*
Function Name: Home Wire Speed
Inputs: Board number
Outputs/Updates: Updates wire speed knob to home position.
Function Description: Turns the wire speed settings stepper motor until the homing switch is reached. This is so that the system will know where the wire speed setting is at any given time.
*/
static void HomeWireSpeed()
{
	uint counter = 5;
	uint direction[2] = { DIRECTIONMASK, 0 };
	uint PulsePin[2] = { PULSEMASK, 0 };
	uint pinstate[2] = { WIRESPEEDHOMEMASK, 0 };

	S826_DioOutputWrite(BRD, pinstate, 1);		// Clear the wire speed home pin.
	Sleep(1);									// wait one millisecond in order to avoid Read/Write errors
	S826_DioInputRead(BRD, pinstate);			// read the current state of the wire speed home pin
	pinstate[0] &= WIRESPEEDHOMEMASK;			// Mask the value read down to just he wire speed pin
	S826_DioOutputSourceWrite(BRD, PulsePin);	// Write the pulse mask to the output

	if (pinstate[0] != WIRESPEEDHOMEMASK)		// check if the state of the wire speed home pin is NOT equal to the homing mask (i.e. the machine is not at it's home position)
	{
		S826_DioOutputWrite(BRD, direction, 1);	// clear the current direction pins
		S826_DioOutputWrite(BRD, direction, 2);	// write the direction needed to the direction pins.
		PwmGeneratorStart(5000, 5000);			// turn on the PWM generator for a 5000 on / 5000 off millisecond cycle time
		while (pinstate[0] != WIRESPEEDHOMEMASK)// check if the state of the wire speed home pin is NOT equal to the homing mask (i.e. the machine is not at it's home position) 
		{
			S826_DioInputRead(BRD, pinstate);	// read the current state of the wire speed home pin
			pinstate[0] &= WIRESPEEDHOMEMASK;	// mask the value down to just the wire speed home pin.
		}
		PwmGeneratorStop();						// turn the PWM generator off
	}
}

/*
Function Name: User Prompt
Inputs: Pointer to Wire Speed Setting variable, and Pointer to Nominal Droplet Time Variable
Outputs/Updates: Updates the Wire Speed Setting Variable, Nominal Droplet Time Variable.
Function Description: Asks the user to input the CNC Speed for the print, Function uses the input value to set the wire speed variable and related nominal droplet time variables for use in the main line.
*/
static void UserPrompt(float *WireSpeedSetting)
{

	float CNCSpeed = 0.0f;

#ifndef _LINUX	
	printf("\n\nPlease press any key to continue...\n");												// asks user to press a key to move on from previous work
	while (!_kbhit());																					// wait for user key press
	_getch();
#endif
Prompt:
	printf("Please input your CNC Speed, Note speed must be between 5.2 in/s and 5.8 in/s:\n ");		// Asks user to input CNC speed value between 5.2 and 5.8 for use calculating the wire speed.
	scanf_s("%f", &CNCSpeed);																			// Scan for user input float
	while (CNCSpeed < MINCNCSETTING || CNCSpeed > MAXCNCSETTING)										// Check if the input value is between the asked for range. NOTE THIS DOES NOT CHECK FOR CHAR BEING INPUT INSTEAD OF NUMBER. POSSIBLE SOURCE ERROR.
	{
		printf("Error. The CNC speed you have entered is outside the valid range.\n");
		Sleep(1000);
		printf("Please input your CNC Speed, Note speed must be between 5.2 in/s and 5.8 in/s:\n ");
		scanf_s("%f", &CNCSpeed);																		// scan for a new input value.
	}
	
	printf("CNC speed is valid, setting up the system now.\n\n");

	*WireSpeedSetting = 3;																				//Give wire speed a value. This is hard coded to 3 in/s because that seems to work best for the range of CNC speed we're requiring. The Adjustment handles the fine tuning later.
}

/*
Function Name: Calibrate Wire Speed
Function Inputs: Pointer to HomeOffset variable
Function Outputs/Updates: Updates the HomeOffset variable
Function Description: Homes the Wire Speed of the welder and triggers the welder to measure the wire speed at the current home position.
It also checks to see if the user has removed the grounding cable and is prepared to calibrate before beginning the process.
*/
static void CalibrateWireSpeed(float *HomeOffset)
{
	int i = 0;
	float WSpeed = 0.0f;
	float AvgWireSpeed = 0.0f;
	uint OutputClear[2] = { G1G0INPUTMASK, 0 };
	uint WeldOn[2] = { G1OUTACTIVE, 0 };
	printf("Machine will now calibrate wire speed home position ...\n");
	printf("please disconnect the Welder Ground Connection and make sure that the welder is powered on, before continuing.\n");

#ifndef _LINUX	
	printf("\nPress any key to continue to begin calibration ...\n\n");
	while (!_kbhit());															//Check for key press from user before moving on.
	_getch();
#endif
	printf("Calibrating, please wait.\n");

	HomeWireSpeed();															// Home the wire speed knob

	S826_DioOutputWrite(BRD, WeldOn, 1);										//clear the welder's activation pin
	S826_DioOutputWrite(BRD, WeldOn, 2);										//Write to the welder activation pin and turn it on

	Sleep(2000);																//wait 2 seconds to let the motor feeding wire get up to speed
	for (i = 0; i <10; i++)														//go through this loop 10 times, each time:
	{
		WireSpeed(&WSpeed);														// measure the wire speed
		AvgWireSpeed = AvgWireSpeed + WSpeed;									// Calculate the average
		Sleep(250);																// wait 250 mS
	}																		

	S826_DioOutputWrite(BRD, WeldOn, 1);										// Turn the welder off

	*HomeOffset = AvgWireSpeed / 10;											// calculate the home offset
	printf("Home Offset = %f\n", *HomeOffset);									// display the home offset
	Sleep(1000);														
	printf("Calibration Complete...\n\n");
}

/*
Function Name: Get Temp
Inputs: Board number, pointer to a temperature variable
Outputs/Updates: Updates: Temperature variable
Function Description: Function reads from the analog pins associated with the temperature sensor and calculates the current temperature in degrees Celsius before updating the main line Temperature variable.
*/
static void GetTemp(float *Temp)
{
	float ADCTemp;

	AnalogVal(&ADCTemp, TEMPERATURECHAN);			// Measure the temperature

	*Temp = ADCTemp * TEMPSENSORCONSTANT;			// Convert the Temperature Sensor value into Celsius from Voltage
	*Temp = *Temp + 800;							// Calculate the actual temperature being measured by adding 800 degrees Celsius to the measured value because 800C is the minimum for our sensor
}

/*
Function Name: Error Calculation
Inputs: Board number, Droplet time variable, Nominal Droplet Time Variable, and a pointer to a Droplet Error Variable.
Outputs/Updates: Updates: Droplet Error variable
Function Description: Function updates the global average droplet error array with the input droplet error, and calculates the percent error between the nominal droplet time and the input droplet time. This is then stored in the main line's Droplet Error variable.
*/
static void ErrorCalculation(float DropletTime, float *AvgDropletError, float DropletErrors[])
{
	int i;
	float AvgSum = 0;

	for (i = 0; i < (10 - 1); i++)													// loop to shift droplet values out/in of the array THIS IS A FIFO OPERATION
	{
		AvgSum = AvgSum + DropletErrors[i];											// Calculate the average sum of values in the array
		DropletErrors[i] = DropletErrors[i + 1];									// Shift the array left by 1
	}

	DropletErrors[i] = (NOMINALDROPLETTIME - DropletTime) / NOMINALDROPLETTIME;		// Add the most recent measured droplet time to the array after calculating it's error from the nominal value.

	*AvgDropletError = AvgSum / 10;													// calculate final average of the error
	printf("Droplet Time = %f\n", DropletTime);										// display the droplet time added to the array
	printf("Droplet Error = %f\n\n", *AvgDropletError);								// display the average droplet error of the array
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                         MAIN FUNCTION                                                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	uint DutyCycleTimeStamp = 0;
	uint RunTime = 0;														// Calculated total runtime of the below loop
	uint Peaks = 0;															// number of peaks counted in the loop
	uint InputStates[2] = { 0, 0 };											// Array to hold the DIO input pinstates
	uint OutputStates[2];													// Array to hold the DIO output states

	float WireSpeed = 0.0f;													// Variable to pass setting if the Wire Speed knob
	float HomeOffset = 0.0f;												// Initial Wire speed position
	float Temp = 0.0f;														// Variable for Temperature
	float DropletErrors[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };				// Array used to calculate average error
	float DropletTime = 0.0f;												// Variable to hold measured droplet time
	float InitialWireSpeedSetting = 0.0f;									// Variable to hold inital wre speed
	float NominalDropletTime = 0.0f;										// Variable to hold nominal droplet time
	float AvgDropletError = 0.0f;											// Variable to hold average droplet error
	float AdjustedWireSpeed = 0.0f;											// Variable to hold 

	uint StartCNCMask[2] = {STARTCNCMASK, 0};								// Array to used to change pinstate to turn on and off CNC Machine

	int loop = 0;															// main loop counter
	int PrintError = 0;														// Variable for error code

	int errcode = S826_ERR_OK;														
	int boardflags = S826_SystemOpen();      								// open 826 driver and find all 826 boards

	if (boardflags < 0)
	{
		errcode = boardflags;                      							 // problem during open
	}						
	else if ((boardflags & (1 << BRD)) == 0)
	{
		printf("TARGET BOARD NOT FOUND\n");        							 // driver didn't find board you want to use
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////////////
		//                            Main Loop Starts Here                             //
		//////////////////////////////////////////////////////////////////////////////////

		WatchDogEnable();													// Enable Watchdog for internal signal routing of PWM Generator
		CounterFrequencyIni();												// Enable Counter as frequency counter
		CalibrateWireSpeed(&HomeOffset);									// Calibrate WireSpeed
		UserPrompt(&InitialWireSpeedSetting, &NominalDropletTime);			// Prompt user for CNC speed and verify that it is a correct speed value.
		InitialWireSpeedSetting = InitialWireSpeedSetting - HomeOffset;		// Home wire speed setting on welder via stepper motor
		SetWireSpeed(&InitialWireSpeedSetting);								// Set wire speed, which is detemined by both CNC speed and current settings
		printf("System setup complete, moving on to print.\n\n");			// Confirmation of setting the wirespeed.
		Sleep(1000);														// Pause for useability
	
		S826_DioOutputWrite(BRD, StartCNCMask, 1);      					// Start CNC movement
		
		printf("\nWaiting for print to start ...\n\n");						// Indicate that machine is ready to 
		GetInputG0G1(&InputStates[0], PrintError);							// Get current G1G0 state
InitialPosition:															// Label to jump to for inital loop

		while (InputStates[0] != G1INPUTACTIVE) 							// Do nothing while G0 = 1 and G1 = 0
		{
			GetAndUpdateG0G1(&InputStates[0], &OutputStates[0]);			// Check G1G0 state 
			Sleep(1);														// Short pause to avoid Read/Write timing violation
		}
		//printf("Found G1\n\n");											// Confirm that G1 has been sensed. For debugging.
		HaltCncAndWeld();      												// Halt CNC movement and welder to prepare for temperature read

		GetTemp(&Temp);     												// Measure temperature
		while (Temp > HIGHTEMPTHRESHOLD)									// When the temp is too hot
		{
			////////////////////////////////////////////////////////
			// Run Torch Routine: This is a manual process for now//
			////////////////////////////////////////////////////////
			GetTemp(&Temp);													// Measure temperature
			printf("Current temperature:, %f\n)", Temp);					// Diaplay Temp

		}

		S826_TimestampRead(BRD, &DutyCycleTimeStamp);    					// Get TimeStamp
		GetAndUpdateG0G1(&InputStates, &OutputStates);      				// Use relay to start welder
		DropletSpacing(NULL, SPIKECHECK);      								// Read current spikes, If current spikes = 0, ERROR. Stop welder and CNC. Exit Program


		S826_DioOutputWrite(BRD, StartCNCMask, 1);      					// Start CNC movement

		do
		{																	
			GetAndUpdateG0G1(&InputStates[0], &OutputStates[0]);			// Update G1G0 State
		} while (InputStates[0] != G1INPUTACTIVE);							// While G1 is not asserted & G0 is asserted




		while (InputStates[0] == G1INPUTACTIVE)								// While G1 is asserted & G0 is not asserted
		{
			DropletSpacing(&DropletTime, SPIKEMEASURE);          			// Measure droplet spacing

			ErrorCalculation(DropletTime, &AvgDropletError, DropletErrors);	// Calculate error from nominal droplet spacing
			if (loop >= 10)													// Wait till average error array fills
			{																// Once average error array is full
				if (AvgDropletError >= .6f || AvgDropletError <= -.6f)		// If the error is more or less than +/-60%
				{
					HaltCncAndWeld();										// Stop the CNC Machine and Welder		
					PrintError = 2;											// Set error code
					ErrorDecode(PrintError);								// Display error and end program
				}
				else (AvgDropletError >= 0.1f || AvgDropletError <= -0.1f)	// If Average Error is above or below +/-10%
				{
					AdjustedWireSpeed = AvgDropletError * -1;				// Scale and negate value for negative feedback.
					SetWireSpeed(&AdjustedWireSpeed);						// Adjust the wirespeed, proportional to the error


				}
			}

			ElapsedTime(DutyCycleTimeStamp, &time);          				// Read TimeStamp for duty cycle check

			if (time > WELDDUTYCYCLE)										// If welder has been running longer than appropiate duty cycle 
			{
				HaltCncAndWeld();											// Stop CNC and Welder
				time = 0;													// Reset Time
				S826_TimestampRead(BRD, &DutyCycleTimeStamp);				// Read Timestamp
				printf("Welder has been running for longer than its duty cycle, waiting for cooldown now.\n");
				while (time < 480000000)									// Wait for the welder cooldown time
				{
					ElapsedTime(DutyCycleTimeStamp, &time);					// Calculate elapsed time
				}
				printf("Welder has cooled down, checking temperature of weld now before proceeding with print.\n");
				goto InitialPosition;										// Jump to chek temp and resart CNC Machine
			}

			GetAndUpdateG0G1(&InputStates[0], &OutputStates[0]);			// Check G1G0 state
			if (InputStates[0] == G0INPUTACTIVE)							// If G0 is asserted and G1 is not asserted
			{
				goto InitialPosition;										
			}
			loop++;															// Increment main loop count
		}
		// End Program
	}
	//////////////////////////////////////////////////
	/////////Sensoray error decoding./////////////////
	//////////////////////////////////////////////////
	switch (errcode)
	{
	case S826_ERR_OK:           break;
	case S826_ERR_BOARD:        printf("Illegal board number"); break;
	case S826_ERR_VALUE:        printf("Illegal argument"); break;
	case S826_ERR_NOTREADY:     printf("Device not ready or timeout"); break;
	case S826_ERR_CANCELLED:    printf("Wait cancelled"); break;
	case S826_ERR_DRIVER:       printf("Driver call failed"); break;
	case S826_ERR_MISSEDTRIG:   printf("Missed adc trigger"); break;
	case S826_ERR_DUPADDR:      printf("Two boards have same number"); break; S826_SafeWrenWrite(BRD, 0x02);
	case S826_ERR_BOARDCLOSED:  printf("Board not open"); break;
	case S826_ERR_CREATEMUTEX:  printf("Can't create mutex"); break;
	case S826_ERR_MEMORYMAP:    printf("Can't map board"); break;
	default:                    printf("Unknown error"); break;
	}

//////////////////////////////////////////////////////////////////////////
////////////////////Close system and free up resources////////////////////
//////////////////////////////////////////////////////////////////////////

#ifndef _LINUX	
	printf("\nKeypress to exit ...\n\n");
	while (!_kbhit());
	_getch();
#endif

	S826_SystemClose();
	return 0;
}

