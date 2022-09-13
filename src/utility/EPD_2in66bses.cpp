/*****************************************************************************
* | File      	:   EPD_2in66bses.cpp
* | Author      :   Kimi based on Waveshare team's sample code
* | Function    :   2.66inch e-paper wbr from SES VUSION 2.6 BWR GL420
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
#include "EPD_2in66bses.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN66BSES_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2IN66BSES_SendCommand(UBYTE Reg)
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
static void EPD_2IN66BSES_SendData(UBYTE Data)
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
static void EPD_2IN66BSES_ReadBusy(void)
{
    Debug("SES266 e-Paper busy\r\n");
    DEV_Delay_ms(50);
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 0) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(10);
    }
    DEV_Delay_ms(50);
    Debug("SES266 e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN66BSES_TurnOnDisplay(void)
{
    EPD_2IN66BSES_SendCommand(0x12);
    EPD_2IN66BSES_ReadBusy();
}
/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN66BSES_Init(void)
{
    EPD_2IN66BSES_Reset();

    EPD_2IN66BSES_SendCommand(0x06); // BOOSTER_SOFT_START
    EPD_2IN66BSES_SendData(0x17);
    EPD_2IN66BSES_SendData(0x17);
    EPD_2IN66BSES_SendData(0x17);
	
    EPD_2IN66BSES_SendCommand(0x04); // POWER_ON
    Debug("SES266 POWER ON CHK Busy\r\n");
    EPD_2IN66BSES_ReadBusy();
	
    EPD_2IN66BSES_SendCommand(0x00); // PANEL_SETTING
    EPD_2IN66BSES_SendData(0xCF);
	
    EPD_2IN66BSES_SendCommand(0x50); // VCOM_AND_DATA_INTERVAL_SETTING
    EPD_2IN66BSES_SendData(0x77);

    EPD_2IN66BSES_SendCommand(0x61); // RESOLUTION_SETTING
    //EPD_2IN66BSES_SendData(EPD_2IN66BSES_WIDTH); // width: 152
    //EPD_2IN66BSES_SendData(EPD_2IN66BSES_HEIGHT >> 8); // height: 292
    //EPD_2IN66BSES_SendData(EPD_2IN66BSES_HEIGHT & 0xFF);
	EPD_2IN66BSES_SendData(0x98);
	EPD_2IN66BSES_SendData(0x01);
	EPD_2IN66BSES_SendData(0x28);

    EPD_2IN66BSES_SendCommand(0x82);
    EPD_2IN66BSES_SendData(0x0A);
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN66BSES_Display(UBYTE *ImageBlack, UBYTE*ImageRed)
{
    UWORD Width, Height;
    Width = (EPD_2IN66BSES_WIDTH % 8 == 0)? (EPD_2IN66BSES_WIDTH / 8 ): (EPD_2IN66BSES_WIDTH / 8 + 1);
    Height = EPD_2IN66BSES_HEIGHT;

    EPD_2IN66BSES_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66BSES_SendData(ImageBlack[i + j * Width]);
        }
    }
	EPD_2IN66BSES_SendCommand(0x92);
	
    EPD_2IN66BSES_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66BSES_SendData(~ImageRed[i + j * Width]);
        }
    }
    EPD_2IN66BSES_SendCommand(0x92);

    EPD_2IN66BSES_TurnOnDisplay();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN66BSES_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN66BSES_WIDTH % 8 == 0)? (EPD_2IN66BSES_WIDTH / 8 ): (EPD_2IN66BSES_WIDTH / 8 + 1);
    Height = EPD_2IN66BSES_HEIGHT;

    EPD_2IN66BSES_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66BSES_SendData(0xff);
        }
    }
	EPD_2IN66BSES_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN66BSES_SendData(0xff);
        }
    }
    EPD_2IN66BSES_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN66BSES_Sleep(void)
{
    EPD_2IN66BSES_SendCommand(0x02);
    EPD_2IN66BSES_ReadBusy();
    EPD_2IN66BSES_SendCommand(0x07); // DEEP_SLEEP
    EPD_2IN66BSES_SendData(0xA5); // check code
}
