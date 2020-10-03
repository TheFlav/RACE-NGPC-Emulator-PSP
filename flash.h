/*---------------------------------------------------------------------------
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version. See also the license.txt file for
 *	additional informations.
 *---------------------------------------------------------------------------
 */

/*
 * Flash chip emulation by Flavor
 *   with ideas from Koyote (who originally got ideas from Flavor :)
 * for emulation of NGPC carts
 */

#define NO_COMMAND              0x00
#define COMMAND_BYTE_PROGRAM    0xA0
#define COMMAND_BLOCK_ERASE     0x30
#define COMMAND_CHIP_ERASE      0x10
#define COMMAND_INFO_READ       0x90

/* what command are we currently on (if any) */
extern unsigned char currentCommand; 

void flashChipWrite(unsigned int addr, unsigned char data);
void vectFlashWrite(unsigned char chip, unsigned int to,
      unsigned char *fromAddr, unsigned int numBytes);
void vectFlashErase(unsigned char chip, unsigned char blockNum);
void vectFlashChipErase(unsigned char chip);
void flashShutdown(void);

#ifdef __cplusplus
extern "C" {
#endif
void setFlashSize(unsigned int romSize);
unsigned char flashReadInfo(unsigned int addr);
#ifdef __cplusplus
}
#endif


extern unsigned char needToWriteFile;
void writeSaveGameFile(void);

/* we found a dirty one, so write the file */
#define WRITE_SAVEGAME_IF_DIRTY if(needToWriteFile) writeSaveGameFile(); 

