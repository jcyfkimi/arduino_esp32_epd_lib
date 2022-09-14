/*****************************************************************************
* | File      	:   EPD_3in71.cpp
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
#include "EPD_3in71.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_3IN71_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(100);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_3IN71_SendCommand(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_3IN71_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_3IN71_ReadBusy(void)
{
    unsigned char busy;
    Debug("SES266 e-Paper busy\r\n");
    DEV_Delay_ms(50);
    do
    {
        EPD_3IN71_SendCommand(0x71);
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
        busy =!(busy & 0x01);        
    }
    while(busy);   
    DEV_Delay_ms(200);
    Debug("SES266 e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_3IN71_TurnOnDisplay(void)
{
    EPD_3IN71_SendCommand(0x12);
    DEV_Delay_ms(100);
    EPD_3IN71_ReadBusy();
}
/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_3IN71_Init(void)
{
    EPD_3IN71_Reset();
	
    EPD_3IN71_SendCommand(0x04); // POWER_ON
    Debug("SES266 POWER ON CHK Busy\r\n");
    EPD_3IN71_ReadBusy();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_3IN71_Display(UBYTE *ImageBlack, UBYTE*ImageRed)
{
    UWORD Width, Height;
    Width = (EPD_3IN71_WIDTH % 8 == 0)? (EPD_3IN71_WIDTH / 8 ): (EPD_3IN71_WIDTH / 8 + 1);
    Height = EPD_3IN71_HEIGHT;

    EPD_3IN71_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN71_SendData(ImageBlack[i + j * Width]);
        }
    }
	
    EPD_3IN71_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN71_SendData(~ImageRed[i + j * Width]);
        }
    }

    EPD_3IN71_TurnOnDisplay();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_3IN71_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_3IN71_WIDTH % 8 == 0)? (EPD_3IN71_WIDTH / 8 ): (EPD_3IN71_WIDTH / 8 + 1);
    Height = EPD_3IN71_HEIGHT;

    EPD_3IN71_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN71_SendData(0xff);
        }
    }
	EPD_3IN71_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN71_SendData(0x00);
        }
    }
    EPD_3IN71_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_3IN71_Sleep(void)
{
    EPD_3IN71_SendCommand(0x50);
    EPD_3IN71_SendData(0xf7);
    
    EPD_3IN71_SendCommand(0x02);
    EPD_3IN71_ReadBusy();
    EPD_3IN71_SendCommand(0x07); // DEEP_SLEEP
    EPD_3IN71_SendData(0xA5); // check code
}
