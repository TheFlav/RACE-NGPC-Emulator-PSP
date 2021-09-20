//---------------------------------------------------------------------------
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version. See also the license.txt file for
//	additional informations.
//---------------------------------------------------------------------------

//
// This is the main program entry point
//
#include <stdio.h>

#include "types.h"
#include "main.h"

#include <libretro.h>
#include <streams/file_stream.h>

#include "flash.h"
#include "race-memory.h"
#include "tlcs900h.h"
#include "input.h"

#include "graphics.h"
#include "tlcs900h.h"

#include "neopopsound.h"

#ifdef DRZ80
#include "DrZ80_support.h"
#else
#if defined(CZ80)
#include "cz80_support.h"
#else
#include "z80.h"
#endif
#endif

extern int finscan;
extern int language;
extern int tipo_consola;

extern retro_log_printf_t log_cb;

int		m_bIsActive;
EMUINFO		m_emuInfo;
SYSTEMINFO	m_sysInfo[NR_OF_SYSTEMS];

#define ARRAY_SIZE(a)		(sizeof(a)/sizeof(*(a)))

void mainemuinit(void)
{
   // initialize cpu memory
   mem_init();
   graphics_init();

   // initialize the TLCS-900H cpu
   tlcs_init();

#if defined(CZ80)
   Z80_Init();
#endif
#if defined(DRZ80) || defined(CZ80)
   Z80_Reset();
#else
   z80Init();
#endif

   // if neogeo pocket color rom, act if we are a neogeo pocket color
   tlcsMemWriteB(0x6F91,tlcsMemReadB(0x00200023));
   if (tipo_consola==1)
      tlcsMemWriteB(0x6F91,0x00);

   // pretend we're running in English mode

   //NOTA setting_ngp_language 00 Ingles - 01 Jap
   if (setting_ngp_language == 0)
   {
      tlcsMemWriteB(0x00006F87,0x01);
   }
   if (setting_ngp_language == 1)
   {
      tlcsMemWriteB(0x00006F87,0x00);
   }

   // kludges & fixes
   switch (tlcsMemReadW(0x00200020))
   {
      case 0x0059:	// Sonic
      case 0x0061:	// Metal Slug 2nd
         *get_address(0x0020001F) = 0xFF;
         break;
   }
   ngpSoundOff();
}

static void	SetActive(BOOL bActive)
{
	m_bIsActive = bActive;
}

static void	SetEmu(int machine)
{
	m_emuInfo.machine = machine;
	m_emuInfo.drv = &m_sysInfo[machine];
}

static int initRom(void)
{
   int		i, m;
   char	*licenseInfo   = " BY SNK CORPORATION";
   BOOL	romFound       = TRUE;

   finscan=198;

   if (mainrom[0x000020] == 0x65 || mainrom[0x000020] == 0x93)
      finscan=199;

#if 0
   dbg_print("in openNgp(%s)\n", lpszPathName);
#endif

   // first stop the current emulation
#if 0
   dbg_print("openNgp: SetEmu(NONE)\n");
#endif
   SetEmu(NGPC);
#if 0
   dbg_print("openNgp: SetActive(FALSE)\n");
#endif
   SetActive(FALSE);

   // check NEOGEO POCKET
   // check license info
   for (i=0;i<19;i++)
   {
      if (mainrom[0x000009 + i] != licenseInfo[i])
         romFound = FALSE;
   }
   if (romFound)
   {
      //dbg_print("openNgp: romFound == TRUE\n");
      i = mainrom[0x000023];
      if (i == 0x10 || i == 0x00)
      {
         /* initialize emulation */
         if (i == 0x10)
            m = NGPC;
         else
         {
            // fix for missing Mono/Color setting in Cool Coom Jam SAMPLE rom
            if (mainrom[0x000020] == 0x34 && mainrom[0x000021] == 0x12)
               m = NGPC;
            else
               m = NGP;
         }
         if (tipo_consola==1)
            m = NGP;

         //dbg_print("openNgp: SetEmu(%d)\n", m);
         SetEmu(m);

         //dbg_print("openNgp: Calling mainemuinit(%s)\n", lpszPathName);
         mainemuinit();
         // start running the emulation loop
         //dbg_print("openNgp: SetActive(TRUE)\n");
         SetActive(TRUE);

         // acknowledge opening of the document went fine
         //dbg_print("openNgp: returning success\n");
         return TRUE;
      }

      log_cb(RETRO_LOG_ERROR, "Not a valid or unsupported rom file.\n");
      return FALSE;
   }

   log_cb(RETRO_LOG_ERROR, "Not a valid or unsupported rom file. romFound==FALSE\n");
   return FALSE;
}

static void initSysInfo(void)
{
	m_bIsActive = FALSE;

	m_emuInfo.machine = NGPC;
	m_emuInfo.drv = &m_sysInfo[m_emuInfo.machine];
	m_emuInfo.romSize = 0;

	strcpy(m_emuInfo.RomFileName, "");

	m_sysInfo[NGP].hSize = 160;
	m_sysInfo[NGP].vSize = 152;
	m_sysInfo[NGP].Ticks = 6*1024*1024;

	m_sysInfo[NGPC].hSize = 160;
	m_sysInfo[NGPC].vSize = 152;
	m_sysInfo[NGPC].Ticks = 6*1024*1024;
}

static int strrchr2(const char *src, int c)
{
  size_t len=strlen(src);

  while(len>0)
  {
    len--;
    if(*(src+len) == c)
      return len;
  }

  return 0;
}

int handleInputFile(const char *romName,
		const unsigned char *romData, int romSize)
{
	initSysInfo();  //initialize it all

	if (romData)
	{
		int size = romSize > MAINROM_SIZE_MAX ?
				MAINROM_SIZE_MAX : romSize;

		m_emuInfo.romSize = size;
		memcpy(mainrom, romData, size);
		strcpy(m_emuInfo.RomFileName, romName);
	}
	else
	{
		RFILE *romFile = NULL;
		int64_t size   = 0;

		//get ROM from binary ROM file
		romFile = filestream_open(romName,
				RETRO_VFS_FILE_ACCESS_READ,
				RETRO_VFS_FILE_ACCESS_HINT_NONE);
		if(!romFile)
		{
			log_cb(RETRO_LOG_ERROR, "Couldn't open %s file\n", romName);
			return 0;
		}

		size = filestream_read(romFile, mainrom, MAINROM_SIZE_MAX);
		filestream_close(romFile);

		if (size <= 0)
		{
			log_cb(RETRO_LOG_ERROR, "Couldn't read %s file\n", romName);
			return 0;
		}

		m_emuInfo.romSize = (int)size;
		strcpy(m_emuInfo.RomFileName, romName);
	}

	if (!initRom())
	{
		log_cb(RETRO_LOG_ERROR, "initRom couldn't handle %s file\n", romName);
		return 0;
	}

	setFlashSize(m_emuInfo.romSize);
	return 1;
}
