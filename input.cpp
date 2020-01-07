/* input.cpp: implementation of the input class. */

#include "StdAfx.h"
#include "main.h"
#include "input.h"
#include "memory.h"

/* address where the state of the input device(s) is stored */
unsigned char	ngpInputState = 0;

BOOL InitInput(void)
{
    return TRUE;
}

void FreeInput(void)
{

}
