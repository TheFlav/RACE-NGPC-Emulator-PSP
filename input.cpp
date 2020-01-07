/* input.cpp: implementation of the input class. */

#ifndef __GP32__
#include "StdAfx.h"
#endif
#include "main.h"
#include "input.h"
#include "memory.h"

/* address where the state of the input device(s) is stored */
#if 0
unsigned char	*InputByte = get_address(0x00006F82);
#endif
unsigned char	ngpInputState = 0;
unsigned char	*InputByte = &ngpInputState;

#ifdef __GP32__
#define DIK_UP BUTTON_UP
#define DIK_DOWN BUTTON_DOWN
#define DIK_LEFT BUTTON_LEFT
#define DIK_RIGHT BUTTON_RIGHT
#define DIK_SPACE BUTTON_A
#define DIK_N BUTTON_B
#define DIK_S BUTTON_SELECT
#define DIK_O BUTTON_START
#else
#define DIK_UP SDLK_UP
#define DIK_DOWN SDLK_DOWN
#define DIK_LEFT SDLK_LEFT
#define DIK_RIGHT SDLK_RIGHT
#define DIK_SPACE SDLK_a
#define DIK_N SDLK_b
#define DIK_S SDLK_ESCAPE
#define DIK_O SDLK_SPACE
#endif

#define DOWN(x)	keystates[x]
#ifdef __GP32__
u16 keystates = 0;
#else
Uint8 *keystates = NULL;
#endif

struct joy_range
{
    int minx, maxx, miny, maxy;
} range;

#ifdef __GP32__
void InitInput(void)
#else
BOOL InitInput(HWND hwnd)
#endif
{
    return TRUE;
}

#ifdef __GP32__
extern "C"
{
	int gp_getButton();
	void clearScreen();
}
int zoom=0,zoomy=16;
#endif

void FreeInput(void)
{

}
