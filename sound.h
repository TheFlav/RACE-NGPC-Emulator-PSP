/*---------------------------------------------------------------------------
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version. See also the license.txt file for
 *	additional informations.
 *---------------------------------------------------------------------------
 */

/* sound.h: interface for the sound class. */

#ifndef AFX_SOUND_H
#define AFX_SOUND_H

#include "types.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

int initSound(void);
void soundCleanup(void);

void setHwnd(void);

/* stolen MAME things */
int osd_start_audio_stream(int stereo);
void osd_stop_audio_stream(void);
int osd_update_audio_stream(short *buffer);
void osd_set_mastervolume(int _attenuation);
int osd_get_mastervolume(void);

/* General sound system functions */
void soundStep(int cycles);
void soundOutput(void);

#define NUM_CHANNELS 32

/* Gameboy sound system */
void gbSoundInit(void);
void gbSoundUpdate(void);
void gbSoundWrite(int reg, unsigned char data);

/* Neogeo pocket sound functions */
void ngpSoundStart(void);
void ngpSoundExecute(void);
void ngpSoundOff(void);
void ngpSoundInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif /* !defined(AFX_SOUND_H) */
