/*****************************************************************************
* | File      	:   epd2in66bses-demo.ino
* | Author      :   Kimi based on Waveshare team's sample code
* | Function    :   2.66inch e-paper wbr from SES VUSION 2.6 BWR GL420 arduino demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-09-13
* | Info        :
* | Tested epd  :   BE2266ES0550ET7AMY01125
* |                 TC026SC1C3-S5(AE2266ES0550EZ7B22009TY)
* -----------------------------------------------------------------------------
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/

#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>

void setup() {
    printf("EPD_2IN66BSES_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN66BSES_Init();
    EPD_2IN66BSES_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage, *RedImage;

    UWORD Imagesize = ((EPD_2IN66BSES_WIDTH % 8 == 0)? (EPD_2IN66BSES_WIDTH / 8 ): (EPD_2IN66BSES_WIDTH / 8 + 1)) * EPD_2IN66BSES_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        while(1);
    }
    if((RedImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        while(1);
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN66BSES_WIDTH, EPD_2IN66BSES_HEIGHT, 270, WHITE);
    Paint_NewImage(RedImage, EPD_2IN66BSES_WIDTH, EPD_2IN66BSES_HEIGHT, 270, WHITE);
	
#if 1   //show image for array    
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in66bb);

    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in66br);
	
    EPD_2IN66BSES_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  
    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);

    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

    Paint_DrawString_CN(130, 0,"你好abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "微雪电子", &Font24CN, WHITE, BLACK);

    EPD_2IN66BSES_Display(BlackImage, RedImage);
    DEV_Delay_ms(4000);   
#endif
	printf("Clear...\r\n");
	EPD_2IN66BSES_Clear();

	printf("Goto Sleep...\r\n");
	EPD_2IN66BSES_Sleep();

	free(BlackImage);
	BlackImage = NULL;
	free(RedImage);
	RedImage = NULL;
	// close 5V
	printf("close 5V, Module enters 0 power consumption ...\r\n");    
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  //
}
