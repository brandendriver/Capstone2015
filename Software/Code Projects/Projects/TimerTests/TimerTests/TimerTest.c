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

#ifndef _LINUX
#include "826api.h"
#else
#include "826api.h"
#endif


int ElapsedTime(uint TimeStampOld, uint board, uint *time, uint64_t Tmax, uint k){

	uint TimeStampNew;

	S826_TimestampRead(board, &TimeStampNew);
	if (TimeStampOld > TimeStampNew){
		time = Tmax - TimeStampOld + TimeStampNew;
		k = 0;
	}
	else{
		time = TimeStampNew - TimeStampOld;
	}
	//time = time / 1000;
	//return time;
}

int main(void){
	uint brd = 0;
	uint TimeStampOld = 0;
	uint time = 0;
	uint k = 1;
	uint64_t Tmax = 4294967296;

	int i = 1;

	int errcode = S826_ERR_OK;

	int boardflags = S826_SystemOpen();									   // open 826 driver and find all 826 boards

	if (boardflags < 0)
		errcode = boardflags;											   // problem during open
	else if ((boardflags & (1 << brd)) == 0)
		printf("TARGET BOARD NOT FOUND\n");								   // driver didn't find board you want to use
	else
	{
		while(k==1){
			S826_TimestampRead(brd, &TimeStampOld);

			time = ElapsedTime(TimeStampOld, brd, &time, Tmax, k);
			//t = t / 1000;
			printf("Old Time Stamp: %u us", TimeStampOld);
			printf("Elapsed Time: %d microseconds\n", time);
		}

	}
	system("pause");

	return 0;
}