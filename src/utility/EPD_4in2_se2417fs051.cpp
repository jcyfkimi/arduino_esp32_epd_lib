/*****************************************************************************
* | File      	:   EPD_4in2_se2417fs051.c
* | Author      :   Waveshare team
* | Function    :   4.2inch e-paper
* | Info        :   For BWR EPD SE2417FS051
*----------------
* |	This version:   V1.0
* | Date        :   2020-11-27
* | Info        :
#
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
#include "EPD_4in2_se2417fs051.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_4IN2_SE2417_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(10);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(10);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_4IN2_SE2417_SendCommand(UBYTE Reg)
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
static void EPD_4IN2_SE2417_SendData(UBYTE Data)
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
void EPD_4IN2_SE2417_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 0) {      //0:BUSY, 1:FREE
        DEV_Delay_ms(100);
    }
    Debug("e-Paper busy release\r\n");
    DEV_Delay_ms(50);
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_4IN2_SE2417_TurnOnDisplay(void)
{
    EPD_4IN2_SE2417_SendCommand(0x12); // DISPLAY_REFRESH
    DEV_Delay_ms(10);
    EPD_4IN2_SE2417_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_4IN2_SE2417_Init(void)
{
    EPD_4IN2_SE2417_Reset();

    EPD_4IN2_SE2417_SendCommand(0x06);
    EPD_4IN2_SE2417_SendData(0x17);
    EPD_4IN2_SE2417_SendData(0x17);
    EPD_4IN2_SE2417_SendData(0x17);
    
    EPD_4IN2_SE2417_SendCommand(0x04);
    EPD_4IN2_SE2417_ReadBusy();
    
    EPD_4IN2_SE2417_SendCommand(0x00);
    EPD_4IN2_SE2417_SendData(0x0f);
    EPD_4IN2_SE2417_SendData(0x0d);

    EPD_4IN2_SE2417_SendCommand(0x50);
    EPD_4IN2_SE2417_SendData(0x77);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_4IN2_SE2417_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_4IN2_SE2417_WIDTH % 8 == 0)? (EPD_4IN2_SE2417_WIDTH / 8 ): (EPD_4IN2_SE2417_WIDTH / 8 + 1);
    Height = EPD_4IN2_SE2417_HEIGHT;

    EPD_4IN2_SE2417_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_SE2417_SendData(0xff);
        }
    }

    EPD_4IN2_SE2417_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_SE2417_SendData(0xff);
        }
    }

    EPD_4IN2_SE2417_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_4IN2_SE2417_Display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_4IN2_SE2417_WIDTH % 8 == 0)? (EPD_4IN2_SE2417_WIDTH / 8 ): (EPD_4IN2_SE2417_WIDTH / 8 + 1);
    Height = EPD_4IN2_SE2417_HEIGHT;

    EPD_4IN2_SE2417_SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_SE2417_SendData(blackimage[i + j * Width]);
        }
    }

    EPD_4IN2_SE2417_SendCommand(0x13);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2_SE2417_SendData(ryimage[i + j * Width]);
        }
    }

    EPD_4IN2_SE2417_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_4IN2_SE2417_Sleep(void)
{

    EPD_4IN2_SE2417_SendCommand(0X02);  	//power off
    EPD_4IN2_SE2417_ReadBusy(); //waiting for the electronic paper IC to release the idle signal
    delay(1000);
    EPD_4IN2_SE2417_SendCommand(0X07);  	//deep sleep
    EPD_4IN2_SE2417_SendData(0xA5);
}
