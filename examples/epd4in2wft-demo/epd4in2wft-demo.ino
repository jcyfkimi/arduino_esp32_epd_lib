/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>

/* Entry point ----------------------------------------------------------------*/
void setup()
{
  printf("EPD_4IN2WFT_test Demo\r\n");
  DEV_Module_Init();

  printf("e-Paper Init and Clear...\r\n");
  EPD_4IN2WFT_Init();
  EPD_4IN2WFT_Clear();
  DEV_Delay_ms(500);

  EPD_4IN2WFT_Display(gImage_4in2depg_BW);
  DEV_Delay_ms(500);

  EPD_4IN2WFT_Init();
  EPD_4IN2WFT_Clear();

  printf("Goto Sleep...\r\n");
  EPD_4IN2WFT_Sleep();
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  //
}
