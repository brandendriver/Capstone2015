#ifndef _LINUX
#include <windows.h>
#include <conio.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef _LINUX
#include "..\826api.h"
#else
#include "826api.h"
#endif


// Helpful macros for DIOs
#define DIO(C)                  ((uint64)1 << (C))                      // convert dio channel number to uint64 bit mask
#define DIOMASK(N)              {(uint)(N) & 0xFFFFFF, (uint)(N)>>24)}  // convert uint64 mask to uint[2] array
#define DIOSTATE(STATES.CHAN)   ((STATES[CHAN / 24] >> (CHAN % 24)) & 1)// extract dio channel's boolean state from uint[2] array

#define true 1
#define false 0

int k = 1;  

int S826_DioOutputWrite(

uint board, // board identifier                             //Turning welder off
uint data[2], // pointer to DIO data
uint mode[1] // 0=write, 1=clear bits, 2=set bits

);

while (k=1)
{
    

int S826_DioInputRead {

uint board,		    // board identifier                     //Read G0 & G1
uint data[2]		// pointer to data buffer

       
    };
    
    if (data[0]==0 && data[1]==1)
    
    {
        k=0
        }
        
        else if (data[0]==1 && data[1]==0)
            
        {
            
            }
            
        else {}
}

