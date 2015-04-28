#ifndef _LINUX
#include <windows.h>
#include <conio.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
	uint diomask1[] = DIOMASK(DIO(diochan1));
	uint diomask2[] = DIOMASK(DIO(diochan2));
	int errcode = S826_ERR_OK;
	int boardflags = S826_SystemOpen();
	int k = 1;


	while (k == 1){
	
	S826_DioOutputWrite(board, diomask1, 2);
	S826_DioInputRead(board, diostates);
	

	printf("%d \t %d \n", DIOSTATE(diostates, diochan1), DIOSTATE(diostates, diochan2));
	
	}


	S826_SystemClose();
	system("pause");
	return 0;
}
