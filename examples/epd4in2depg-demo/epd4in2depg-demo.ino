/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>

/* Entry point ----------------------------------------------------------------*/
void setup()
{
  printf("EPD_4IN2DEPGWFT_test Demo\r\n");
  DEV_Module_Init();

  printf("e-Paper Init and Clear...\r\n");
  EPD_4IN2DEPG_Init();
  EPD_4IN2DEPG_Clear();
  DEV_Delay_ms(500);

  EPD_4IN2DEPG_Display(gImage_4in2depg_BW, gImage_4in2depg_R);
  DEV_Delay_ms(500);

  EPD_4IN2DEPG_Init();
  EPD_4IN2DEPG_Clear();

  printf("Goto Sleep...\r\n");
  EPD_4IN2DEPG_Sleep();
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  //
}
