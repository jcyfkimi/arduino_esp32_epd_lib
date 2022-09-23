/*****************************************************************************
* | File      	:   EPD_4IN2depg.cpp
* | Author      :   Kimi based on Waveshare team's sample code
* | Function    :   4.2inch DEPG0420BNS e-paper
* | Info        :
*----------------
* |	This version:   V3.1
* | Date        :   2022-09-23
* | Info        :
* | Tested epd  :   DEPG0420BNS19AF
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
#include "EPD_4IN2depg.h"
#include "Debug.h"


/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_4IN2DEPG_Reset(void)
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
static void EPD_4IN2DEPG_SendCommand(UBYTE Reg)
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
static void EPD_4IN2DEPG_SendData(UBYTE Data)
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
void EPD_4IN2DEPG_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(100);
    }
	DEV_Delay_ms(200);  
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_4IN2DEPG_TurnOnDisplay(void)
{
	EPD_4IN2DEPG_SendCommand(0x22);		 //DISPLAY REFRESH 	
    EPD_4IN2DEPG_SendData(0xf7);
    EPD_4IN2DEPG_SendCommand(0x20);	
	EPD_4IN2DEPG_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/

//UC8176C
void EPD_4IN2DEPG_Init(void)
{
	EPD_4IN2DEPG_Reset();
    EPD_4IN2DEPG_ReadBusy();
    EPD_4IN2DEPG_SendCommand(0x12);         // SWRESET
    EPD_4IN2DEPG_ReadBusy();
	EPD_4IN2DEPG_SendCommand(0x01);			// Set MUX as 300 
	EPD_4IN2DEPG_SendData(0x2b);	          
	EPD_4IN2DEPG_SendData(0x01);
    EPD_4IN2DEPG_SendData(0x00);

	EPD_4IN2DEPG_SendCommand(0x11);         // Data entry mode
	EPD_4IN2DEPG_SendData (0x01);   

	EPD_4IN2DEPG_SendCommand(0x44);         
    EPD_4IN2DEPG_SendData(0x00);            // RAM x address start at 0
    EPD_4IN2DEPG_SendData(0x31);            // RAM x address end at 31h(49+1)*8->400
    EPD_4IN2DEPG_SendCommand(0x45); 
    EPD_4IN2DEPG_SendData(0x2b);            // RAM y address start at 12Bh
    EPD_4IN2DEPG_SendData(0x01);
    EPD_4IN2DEPG_SendData(0x00);             // RAM y address end at 00h
    EPD_4IN2DEPG_SendData(0x00);    


	EPD_4IN2DEPG_SendCommand(0x3c);			// board
	EPD_4IN2DEPG_SendData(0x01);		

    EPD_4IN2DEPG_SendCommand(0x18);			
	EPD_4IN2DEPG_SendData(0x80);

	EPD_4IN2DEPG_SendCommand(0x4e);			
	EPD_4IN2DEPG_SendData (0x00);      	
	EPD_4IN2DEPG_SendCommand(0x4f);			
	EPD_4IN2DEPG_SendData (0x2b);
    EPD_4IN2DEPG_SendData (0x01);
}	
/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_4IN2DEPG_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_4IN2DEPG_WIDTH % 8 == 0)? (EPD_4IN2DEPG_WIDTH / 8 ): (EPD_4IN2DEPG_WIDTH / 8 + 1);
    Height = EPD_4IN2DEPG_HEIGHT;

    EPD_4IN2DEPG_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2DEPG_SendData(0xFF);
        }
    }
    
    EPD_4IN2DEPG_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2DEPG_SendData(0x00);
        }
    }

    EPD_4IN2DEPG_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_4IN2DEPG_Display(const UBYTE *ImageBlack, const UBYTE *ImageRed)
{
    UWORD Width, Height;
    Width = (EPD_4IN2DEPG_WIDTH % 8 == 0)? (EPD_4IN2DEPG_WIDTH / 8 ): (EPD_4IN2DEPG_WIDTH / 8 + 1);
    Height = EPD_4IN2DEPG_HEIGHT;

    EPD_4IN2DEPG_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2DEPG_SendData(ImageBlack[i + j * Width]);
        }
    }
    
    EPD_4IN2DEPG_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_4IN2DEPG_SendData(~ImageRed[i + j * Width]);
        }
    }

	EPD_4IN2DEPG_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_4IN2DEPG_Sleep(void)
{
	EPD_4IN2DEPG_SendCommand(0x10); // DEEP_SLEEP
    EPD_4IN2DEPG_SendData(0X01);
    DEV_Delay_ms(100);  
}