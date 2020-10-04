#include <stdint.h>
#include "race-memory.h"

extern "C" {

/* standard VRAM table adresses */
unsigned char *sprite_table   = get_address(0x00008800);
unsigned char *pattern_table   = get_address(0x0000A000);
unsigned short*patterns = (unsigned short*)pattern_table;
unsigned short *tile_table_front  = (unsigned short *)get_address(0x00009000);
unsigned short *tile_table_back  = (unsigned short *)get_address(0x00009800);
unsigned short *palette_table   = (unsigned short *)get_address(0x00008200);
unsigned char *bw_palette_table  = get_address(0x00008100);
unsigned char *sprite_palette_numbers = get_address(0x00008C00);

/* VDP registers
 *
 * where is the vdp rendering now on the lcd display?
 */

#if 0
unsigned char *scanlineX  = get_address(0x00008008);
#endif
unsigned char *scanlineY  = get_address(0x00008009);
/* frame 0/1 priority registers */
unsigned char *frame0Pri  = get_address(0x00008000);
unsigned char *frame1Pri  = get_address(0x00008030);
/* windowing registers */
unsigned char *wndTopLeftX = get_address(0x00008002);
unsigned char *wndTopLeftY = get_address(0x00008003);
unsigned char *wndSizeX  = get_address(0x00008004);
unsigned char *wndSizeY  = get_address(0x00008005);
/* scrolling registers */
unsigned char *scrollSpriteX = get_address(0x00008020);
unsigned char *scrollSpriteY = get_address(0x00008021);
unsigned char *scrollFrontX = get_address(0x00008032);
unsigned char *scrollFrontY = get_address(0x00008033);
unsigned char *scrollBackX = get_address(0x00008034);
unsigned char *scrollBackY = get_address(0x00008035);
/* background color selection register and table */
unsigned char *bgSelect  = get_address(0x00008118);
unsigned short *bgTable  = (unsigned short *)get_address(0x000083E0);
unsigned char *oowSelect  = get_address(0x00008012);
unsigned short *oowTable  = (unsigned short *)get_address(0x000083F0);
/* machine constants */
unsigned char *color_switch = get_address(0x00006F91);

unsigned char *rasterY = get_address(0x00008035);

}
