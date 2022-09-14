/*****************************************************************************
* | File      	:   EPD_3in71.h
* | Author      :   Kimi based on Waveshare team's sample code
* | Function    :   3.71 inch e-paper BWR
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-02
* | Info        :
* | Tested epd  :   Unknow epd, FPC labeled as "FPC-2303", which compactable with gooddisplay GDEY037Z03
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
#ifndef __EPD_3IN71_H_
#define __EPD_3IN71_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_3IN71_WIDTH       240
#define EPD_3IN71_HEIGHT      416


void EPD_3IN71_Init(void);
void EPD_3IN71_Display(UBYTE *ImageBlack, UBYTE*ImageRed);
void EPD_3IN71_Clear(void);
void EPD_3IN71_Sleep(void);
#endif