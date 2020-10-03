/*---------------------------------------------------------------------------
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version. See also the license.txt file for
 *	additional informations.
 *---------------------------------------------------------------------------
 */

/* graphics.h: interface for the graphics class.
 *
 */

#ifndef AFX_GRAPHICS_H
#define AFX_GRAPHICS_H

#if _MSC_VER > 1000
#pragma once
#endif /* _MSC_VER > 1000 */

/* actual NGPC */
#define NGPC_SIZEX 160
#define NGPC_SIZEY 152

/* render screen 260x152 is good for NGPC */
#define SIZEX	260
#define SIZEY	152

#ifdef __cplusplus
extern "C" {
#endif
BOOL graphics_init(void);
#ifdef __cplusplus
}
#endif
void graphics_paint(void);
/* new renderer (NeoGeo Pocket (Color)) */
void myGraphicsBlitLine(unsigned char render);
void graphicsSetDarkFilterLevel(unsigned filterLevel);

/*
 * adventure vision stuff
 */

extern unsigned short palettes[16*4+16*4+16*4]; /* placeholder for the converted palette */
extern int    totalpalette[32*32*32];
#define NGPC_TO_SDL16(col) totalpalette[col & 0x0FFF]

#define setColPaletteEntry(addr, data) palettes[(addr)] = NGPC_TO_SDL16(data)
#define setBWPaletteEntry(addr, data) palettes[(addr)] = NGPC_TO_SDL16(data)

extern unsigned char *scanlineY;

struct ngp_screen
{
   int w, h;                 
   void *pixels;
};

#endif /* !defined(AFX_GRAPHICS_H) */
