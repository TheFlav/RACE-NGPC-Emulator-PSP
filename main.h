#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <ctype.h>
#include <math.h>

typedef struct SYSTEMINFO
{
	int		hSize;
	int		vSize;
	int		Ticks;
	int		InputKeys[12];
} SYSTEMINFO;

typedef struct EMUINFO
{
	char 	RomFileName[_MAX_PATH];

	int		machine;		// what kind of machine should we emulate
	int		romSize;		// what is the size of the currently loaded file
	int		samples;
	SYSTEMINFO	*drv;
} EMUINFO;

#define KEY_UP			0
#define KEY_DOWN		1
#define KEY_LEFT		2
#define KEY_RIGHT		3
#define KEY_START		4
#define KEY_BUTTON_A	5
#define KEY_BUTTON_B	6
#define KEY_SELECT		7
#define KEY_UP_2		8
#define KEY_DOWN_2		9
#define KEY_LEFT_2		10
#define KEY_RIGHT_2		11

// Possible Neogeo Pocket versions
#define NGP				0x00
#define NGPC			0x01

#define NR_OF_SYSTEMS	2


extern int		m_bIsActive;
extern EMUINFO		m_emuInfo;
extern SYSTEMINFO	m_sysInfo[NR_OF_SYSTEMS];
extern int romSize;

int handleInputFile(char *romName);
void mainemuinit(void);

/* to call these FPS is a bit of a misnomer */
#define HOST_FPS 60  /* the number of frames we want to draw to the host's screen every second */

#endif
