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

///////////////////////////////////////////////////////////////////////////////////////////////
// SENSORAY MODEL 826 PROGRAMMING EXAMPLES
// This file contains simple functions that show how to program the 826.
// Copyright (C) 2012 Sensoray
///////////////////////////////////////////////////////////////////////////////////////////////

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



#define X826(FUNC)   if ((errcode = FUNC) != S826_ERR_OK) { printf("\nERROR: %d\n", errcode); return errcode;}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEMO: OVERSAMPLE TWO ANALOG INPUTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int DemoAnalogOversample(uint board)
{
    // Adc timeslot attributes.
    // In each adc burst, convert analog channel 0 eight times then channel 1 eight times. Allow each channel to settle for 50 microseconds
    // after switching (before first conversion); subsequent conversions on the same channel don't need additional settling time.
    struct SLOTATTR {
        uint chan;      // analog input channel
        uint tsettle;   // settling time in microseconds
    } attr[16] = {                                                          // during each adc burst:
        {0, 50}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},    //   switch to channel 0, wait 50us, digitize eight times
        {1, 50}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}};   //  
	x
    int i;
    int slot;
    uint slotlist;
    int adcbuf[16];
    int errcode;    

    printf("\nDemoAnalogOversample\n");

    // Program attributes for all timeslots.
    for (slot = 0; slot < 16; slot++)
        X826( S826_AdcSlotConfigWrite(board, slot, attr[slot].chan, attr[slot].tsettle, S826_ADC_GAIN_1) );

    // Configure adc system and start it running.
    X826( S826_AdcSlotlistWrite(board, 0xFFFF, S826_BITWRITE)   );  // enable all timeslots
    X826( S826_AdcTrigModeWrite(board, 0)                       );  // select continuous (untriggered) mode
    X826( S826_AdcEnableWrite(board, 1)                         );  // enable conversions

    // Fetch and display oversampled data.
    for (i = 0; i < 10; i++)                                        // repeat a few times:
    {
        int sum[] = {0, 0};                                         // zero the adc sample accumulators, one per analog input channel
        slotlist = 0xFFFF;
        X826( S826_AdcRead(board, adcbuf, NULL, &slotlist, 1000) ); // read adc data from 16 slots

        for (slot = 0; slot < 16; slot++)                           // sum the 8 samples from each analog input 
            sum[slot >> 3] += (short)( adcbuf[slot] & 0xFFFF );		// masking off sample meta-data
		
        printf("CH0=%3.3fV, CH1=%3.3fV\n", (double)(sum[0]*10)/(8*32768), (double)(sum[1]*10)/(8*32768));     // compute average values, report measured voltages
    }

    X826( S826_AdcEnableWrite(board, 0)                         );  // halt adc conversions
    return errcode;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main function.

int main(void)
{
    uint board      = 0;                        // change this if you want to use other than board number 0
    int errcode     = S826_ERR_OK;	
    int boardflags  = S826_SystemOpen();        // open 826 driver and find all 826 boards

    if (boardflags < 0)
        errcode = boardflags;                       // problem during open
    else if ((boardflags & (1 << board)) == 0)
        printf("TARGET BOARD NOT FOUND\n");         // driver didn't find board you want to use
    else
    {
        // Execute the demo functions. Uncomment any functions you want to run.
        X826( DemoPeriodicTimer(board)      );      // periodic timer
        // X826( DemoDioEdgeDetect(board)      );      // dio edge detection
        // X826( DemoSinewaveGenerator(board)  );      // analog sinewave output
        // X826( DemoAnalogOversample(board)   );      // oversampled analog inputs
        // X826( DemoAnalogTrack(board)        );      // copy analog input to dac
        // X826( DemoWatchdog(board)           );      // watchdog timer		
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
    case S826_ERR_DUPADDR:      printf("Two boards have same number"); break;S826_SafeWrenWrite(board, 0x02);
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

