////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (C) yyyy  name of author												///
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
// Helpful macros for DIOs
#define DIO(C)                  ((uint64)1 << (C))                          // convert dio channel number to uint64 bit mask
#define DIOMASKA(N)             {(uint)(N) & 0x00C00000, (uint)((N) >> 24)}   // convert uint64 bit mask to uint[2] array
#define DIOMASKB(N)				{(uint)(N) & 0x00C00000, (uint)((N) >> 24)}
#define DIOSTATE(STATES,CHAN)   ((STATES[CHAN / 24] >> (CHAN % 24)) & 1)    // extract dio channel's boolean state from uint[2] array


#define X826(FUNC)   if ((errcode = FUNC) != S826_ERR_OK) { printf("\nERROR: %d\n", errcode); return errcode;}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main function.

int main(void)
{
	uint brd = 0;                        // change this if you want to use other than board number 0
	uint TimeStampOld;					//this is for calculating the elapsed time of a sampling cycle
	uint time = 0;						// this is the elapsed time of a sampling cycle
	uint RunTime = 0;					// Calculated total runtime of the below loop
	uint Peaks = 0;						// number of peaks counted in the loop
	uint diochan1 = 23;					//setting the channel for digital out
	//uint diomask1[] = DIOMASK(DIO(diochan1));		//masking the channel of our digital out

	uint diostatesA[2];
	uint diostatesB[2];
	uint diostatesB_Mask[2] = {0x00300000, 0};

	double AdcValOld = 0;		//first measured value from ADC. Measured off of the current sensor.
	double AdcValNew = 0;		//second measured value from ADC. Compared against AdcValOld to check for peaks

	int k = 1;

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

		while (1==k){

			S826_DioInputRead(brd, diostatesA);
			diostatesA[0] &= 0x00C00000;  //masking to those two bits

			diostatesA[0] = (diostatesA[0] >> 2);
			printf("DIO input after shift: %u\t", diostatesA[0]);
			
			S826_DioOutputRead(brd, diostatesB);
			diostatesB[0] &= 0x00300000;
	
			printf("DIO output after shift: %u\n", diostatesB[0]);

			if ((diostatesB[0] != diostatesA[0])){	//check if SHIFTED input dio is the same as current output dio
				S826_DioOutputWrite(brd, diostatesB_Mask, 1);
				S826_DioOutputWrite(brd, diostatesA, 2);		//if it's not, write the SHIFTED input to output

			}

			//Sleep(1000);
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