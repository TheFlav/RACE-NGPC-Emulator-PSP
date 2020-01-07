/* input.cpp: implementation of the input class. */

#include "StdAfx.h"
#include "main.h"
#include "input.h"
#include "memory.h"

/* address where the state of the input device(s) is stored */
#if 0
unsigned char	*InputByte = get_address(0x00006F82);
#endif
unsigned char	ngpInputState = 0;
unsigned char	*InputByte = &ngpInputState;

#define DIK_UP SDLK_UP
#define DIK_DOWN SDLK_DOWN
#define DIK_LEFT SDLK_LEFT
#define DIK_RIGHT SDLK_RIGHT
#define DIK_SPACE SDLK_a
#define DIK_N SDLK_b
#define DIK_S SDLK_ESCAPE
#define DIK_O SDLK_SPACE

#define DOWN(x)	keystates[x]
Uint8 *keystates = NULL;

struct joy_range
{
    int minx, maxx, miny, maxy;
} range;

BOOL InitInput(void)
{
    return TRUE;
}

void FreeInput(void)
{

}
