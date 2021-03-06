#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pspgu.h>
#include <psptypes.h>
#include <psprtc.h>
#include <pspdisplay.h>
#include "emulate.h"


#include "pl_psp.h"
#include "pl_snd.h"
#include "image.h"
#include "video.h"
#include "pl_perf.h"
#include "pl_file.h"
#include "ctrl.h"
#include "pl_util.h"
#include "pl_rewind.h"
#include <pspctrl.h>

#include "StdAfx.h"
#include "state.h"
#include "neopopsound.h"
#include "input.h"
#include "flash.h"
#include "tlcs900h.h"
#include "memory.h"

psp_ctrl_mask_to_index_map_t physical_to_emulated_button_map[] =
{
  { PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER, 16 },
  { PSP_CTRL_START    | PSP_CTRL_SELECT,   17 },
  { PSP_CTRL_SELECT   | PSP_CTRL_LTRIGGER, 18 },
  { PSP_CTRL_SELECT   | PSP_CTRL_RTRIGGER, 19 },
  { PSP_CTRL_ANALUP,   0 }, { PSP_CTRL_ANALDOWN,  1 },
  { PSP_CTRL_ANALLEFT, 2 }, { PSP_CTRL_ANALRIGHT, 3 },
  { PSP_CTRL_UP,   4 }, { PSP_CTRL_DOWN,  5 },
  { PSP_CTRL_LEFT, 6 }, { PSP_CTRL_RIGHT, 7 },
  { PSP_CTRL_SQUARE, 8 },  { PSP_CTRL_CROSS,     9 },
  { PSP_CTRL_CIRCLE, 10 }, { PSP_CTRL_TRIANGLE, 11 },
  { PSP_CTRL_LTRIGGER, 12 }, { PSP_CTRL_RTRIGGER, 13 },
  { PSP_CTRL_SELECT, 14 }, { PSP_CTRL_START, 15 },
  { 0, -1 }
};

PspImage *Screen;
pl_rewind Rewinder;

extern int m_bIsActive;
extern psp_ctrl_map_t current_map;
extern pl_file_path ScreenshotPath;
extern int changeborder;
extern int gameonram;
extern int primerjuego;
extern int customborder;
int borderc;

/*NOTA*/
extern int res2x;
extern PspImage *borderimg;
extern int vertical;
extern int bilinear;
int diferencia;
extern char *bordername;
extern int sinborde;
const char *border_back;
extern int save_name;
static int fondo;
extern int bordernull;
int tmp1;
int tmp2;
/*NOTA*/


static pl_perf_counter FpsCounter;
static int ScreenX, ScreenY, ScreenW, ScreenH;
static int ClearScreen;
static int Rewinding;
static int frames_until_save;
static u32 TicksPerUpdate;
static u64 LastTick;
static u64 CurrentTick;
static u8 RewindEnabled;

static void AudioCallback(pl_snd_sample* buf,
                          unsigned int samples,
                          void *userdata);

/* Initialize emulation */
int InitEmulation()
{
  if (!(Screen = pspImageCreateVram(256, 152, PSP_IMAGE_16BPP)))
    return 0;

  Screen->Viewport.Width = 160;
  Screen->Viewport.Height = 152;

  sound_system_init();

  /* Initialize rewinder */
  pl_rewind_init(&Rewinder,
    state_store_mem,
    state_restore_mem,
    state_get_size);

  pl_snd_set_callback(0, AudioCallback, NULL);

  return 1;
}

void graphics_paint()
{

  pspVideoBegin();
  
  
  

  
  /* Clear the buffer first, if necessary */
  if (ClearScreen >= 0)
  {
    ClearScreen--;
    pspVideoClearScreen();
  }

  /* Blit screen */
  sceGuDisable(GU_BLEND);
  pspVideoPutImage(Screen, ScreenX, ScreenY, ScreenW, ScreenH);
  
    /*vertical=0;
  Screen->Viewport.Height = 24;
  pspVideoPutImage(Screen, ScreenX, 0, ScreenW, 24);
  vertical=48;
  Screen->Viewport.Height = 136;
  pspVideoPutImage(Screen, ScreenX, 24, ScreenW,224);
  vertical=136;
  Screen->Viewport.Height = 152;
  pspVideoPutImage(Screen, ScreenX, 256, ScreenW,16);*/
  

        SceCtrlData pad;
        sceCtrlReadBufferPositive(&pad, 1);
  
  
  if (psp_options.display_mode==SIMPLE_2X){
        if (pad.Buttons & PSP_CTRL_START)
        {
        tmp1 = vertical;
        tmp2 = Screen->Viewport.Height;
       vertical=0;
        //bilinear = 1;
       Screen->Viewport.Height = 152;
       pspVideoPutImage(Screen, ScreenX, ScreenY, 320, 272); 
       bilinear = 0;
        vertical = tmp1 ;
        Screen->Viewport.Height = tmp2;          
       }}
  
  
  sceGuEnable(GU_BLEND);

  if (fondo<10) {

  if (psp_options.border==1) pspVideoPutImage1(borderimg, 0, 0, 480, 272);
  else  pspVideoPutImage1(borderimg, 0, 0, 0, 0);
  
  fondo++;
  }

  //sceDisplayWaitVblankStart();
 
 
  /* Wait if needed
  
  if (psp_options.pvsync == 0) {
  
  if (psp_options.update_freq)
  {
    do { sceRtcGetCurrentTick(&CurrentTick); }
    while (CurrentTick - LastTick < TicksPerUpdate);
    LastTick = CurrentTick;
    }
  }
  */

  if (psp_options.pvsync == 1)
  {
  sceDisplayWaitVblankStart();
  }
 
  else {
    do { sceRtcGetCurrentTick(&CurrentTick); }
    while (CurrentTick - LastTick < TicksPerUpdate);
    LastTick = CurrentTick;}


  
 
   /* Show FPS counter */
  if (psp_options.show_fps)
  {
   static char fps_display[64];
  sprintf(fps_display, " %3.02f ", pl_perf_update_counter(&FpsCounter));

    int width = pspFontGetTextWidth(&PspStockFont, fps_display);
    int height = pspFontGetLineHeight(&PspStockFont);

    pspVideoFillRect(SCR_WIDTH - width, 0, SCR_WIDTH, height, PSP_COLOR_BLACK);
    pspVideoPrint(&PspStockFont, SCR_WIDTH - width, 0, fps_display, PSP_COLOR_WHITE);
  }
  
  
  pspVideoEnd();

  pspVideoSwapBuffers();

}

/* Run emulation */
void RunEmulation()
{

  //pl_psp_set_clock_freq(psp_options.clock_freq);
  

  float ratio;
  
    res2x = 0;
  /* Recompute screen size/position */
  switch (psp_options.display_mode)
  {
  default:
  case DISPLAY_MODE_UNSCALED:
  
    if (borderc==0){
    borderimg = pspImageLoadPng("border.png");
    borderc=1;
    }
    
    fondo=0;
    res2x = 0;
    bilinear = 0;
    ScreenW = Screen->Viewport.Width;
    ScreenH = Screen->Viewport.Height;
    break;
  case DISPLAY_MODE_FIT_HEIGHT:
  
    if (borderc==0){
    borderimg = pspImageLoadPng("border.png");
    borderc=1;
    }
    fondo=0;
    bilinear = 1;
    res2x = 0;
    ratio = (float)SCR_HEIGHT / (float)Screen->Viewport.Height;
    ScreenW = (int)((float)Screen->Viewport.Width * ratio);
    ScreenH = SCR_HEIGHT;
    break;
  case DISPLAY_MODE_FILL_SCREEN:
  
    if (borderc==0){
    borderimg = pspImageLoadPng("border.png");
    borderc=1;
    }
  
    fondo=20;  
    bilinear = 1;
    ScreenW = SCR_WIDTH;
    ScreenH = SCR_HEIGHT;
    break;
  case SIMPLE_2X:
    res2x = 1;
    bilinear = 0;
    
    if (borderc==0){
    borderimg = pspImageLoadPng("border.png");
    borderc=1;
    }

    fondo=0;
    vertical = psp_options.pos_vert;
    diferencia = (32 - vertical)/2;
    Screen->Viewport.Height = 152-(diferencia);
    ScreenW = (SCR_WIDTH/3)*2;
    ScreenH = SCR_HEIGHT;
    break;
  case SIMPLE_W:
  
    if (borderc==0){
    borderimg = pspImageLoadPng("border.png");
    borderc=1;
    }
  
    fondo=20;
    res2x = 1;
    bilinear = 0;   
    vertical = psp_options.pos_vert;
    diferencia = (32 - vertical)/2;
    Screen->Viewport.Height = 152-(diferencia);
    ScreenW = SCR_WIDTH;
    ScreenH = SCR_HEIGHT;
    break;
  }

  ScreenX = (SCR_WIDTH / 2) - (ScreenW / 2);
  ScreenY = (SCR_HEIGHT / 2) - (ScreenH / 2);

  /* Initialize performance counter */
  pl_perf_init_counter(&FpsCounter);
  ClearScreen = 1;
  frames_until_save = 0;
  Rewinding = 0;

  /* Determine if at least 1 button is mapped to 'rewind' */
  RewindEnabled = 0;
  psp_ctrl_mask_to_index_map_t *current_mapping = physical_to_emulated_button_map;
  for (; current_mapping->mask; current_mapping++)
  {
    u32 code = current_map.button_map[current_mapping->index];
    if ((code & SPC) && (CODE_MASK(code) == SPC_REWIND))
    {
      RewindEnabled = 1; /* Rewind button is mapped */
      break;
    }
  }
  
  /* Recompute update frequency*/
  u32 TicksPerSecond = sceRtcGetTickResolution();
  if (psp_options.update_freq)
  {
    TicksPerUpdate = TicksPerSecond
      / (psp_options.update_freq / (psp_options.frame_skip + 1));
    sceRtcGetCurrentTick(&LastTick);
  } 

  /* Resume sound */
  pl_snd_resume(0);

  /* Resume emulation */
  m_bIsActive = 1;
  ngpc_run();

  /* Pause sound */
  pl_snd_pause(0);
}


void SetVideoMode()
{
  res2x = 0;
    Screen->Viewport.Height = 152;
    ScreenW = Screen->Viewport.Width;
    ScreenH = Screen->Viewport.Height;
  
  ScreenX = (SCR_WIDTH / 2) - (ScreenW / 2);
  ScreenY = (SCR_HEIGHT / 2) - (ScreenH / 2);
  ClearScreen = 1;
  } 


void UpdateInputState()
{
  ngpInputState = 0;

  /* Parse input */
  static int autofire_status = 0;
  static SceCtrlData pad;

  if (pspCtrlPollControls(&pad))
  {
    if (--autofire_status < 0)
      autofire_status = psp_options.autofire;
    psp_ctrl_mask_to_index_map_t *current_mapping = physical_to_emulated_button_map;

    for (; current_mapping->mask; current_mapping++)
    {
      u32 code = current_map.button_map[current_mapping->index];
      u8  on = (pad.Buttons & current_mapping->mask) == current_mapping->mask;

      /* Check to see if a button set is pressed. If so, unset it, so it */
      /* doesn't trigger any other combination presses. */
      if (on) pad.Buttons &= ~current_mapping->mask;

      if (!Rewinding)
      {
        if (code & AFI)
        {
          if (on && (autofire_status == 0)) 
            ngpInputState |= CODE_MASK(code);
          continue;
        }
        else if (code & JST)
        {
          if (on) ngpInputState |= CODE_MASK(code);
          continue;
        }
      }

      if (code & SPC)
      {
        switch (CODE_MASK(code))
        {
        case SPC_MENU:
          if (on) m_bIsActive = 0;
          break;
        case SPC_REWIND:
          Rewinding = on;
          break;
        }
      }
    }
  }
}

void HandleStateSaving()
{
  if (!RewindEnabled)
    return;

  /* Rewind/save state */
  if (!Rewinding)
  {
    if (--frames_until_save <= 0)
    {
      frames_until_save = psp_options.rewind_save_rate;
      pl_rewind_save(&Rewinder);
    }
  }
  else
  {
    frames_until_save = psp_options.rewind_save_rate;
    pl_rewind_restore(&Rewinder);
  }
}

static void AudioCallback(pl_snd_sample* buf,
                          unsigned int samples,
                          void *userdata)
{
  int length_bytes = samples << 1; /* 2 bytes per sample */

  if (!Rewinding)
  {
    sound_update((_u16*)buf, length_bytes); //Get sound data
    dac_update((_u16*)buf, length_bytes);
  }
  else /* Render silence */
  {
    memset(buf, 0, length_bytes);
  }
}

/* Release emulation resources */
void TrashEmulation()
{
  pl_rewind_destroy(&Rewinder);

  flashShutdown();

  pspImageDestroy(Screen);
}

