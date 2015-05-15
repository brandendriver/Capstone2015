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




int main(void)
{
	uint board = 0;
	uint diochan1 = 23;
	uint diochan2 = 22;
	uint diostates[2];
	uint diomask1[2] = DIOMASK(DIO(diochan1));
	uint diomask2[2] = DIOMASK(DIO(diochan2));
	int errcode = S826_ERR_OK;
	int boardflags = S826_SystemOpen();
	int k = 1;


	while (k == 1){
	
	S826_DioOutputWrite(board, diomask1, 0);

	Sleep(1000);

	S826_DioOutputWrite(board, diomask1, 1);
	Sleep(1000);

	//S826_DioInputRead(board, diostates);
	

	//printf("%d \t %d \n", DIOSTATE(diostates, diochan1), DIOSTATE(diostates, diochan2));
	
	}


	S826_SystemClose();
	system("pause");
	return 0;
}
