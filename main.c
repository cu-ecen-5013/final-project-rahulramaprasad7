#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "epd1in54.h"
#include "epdif.h"
#include "Lucida_Console_8pts.h"

uint8_t imageBuf[5000] = {0};
uint8_t imageBuffer[200][25];

#define DISP_WHITE 0
#define DISP_BLACK 1

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void drawPixel(uint8_t x, uint8_t y, bool color)
{
    if(x > 199 || y > 199)
    {
        printf("%s: out of bound for X:%d Y:%d\n", __func__, x, y);
        return;
    }

    uint8_t xPos, xBitPos;

    xPos = x / 8;
    xBitPos = x % 8;
    
    if(color)
        imageBuffer[y][xPos] &= ~(1 << (7 - xBitPos));
    else
        imageBuffer[y][xPos] |= (1 << (7 - xBitPos));

    // imageBuffer[y][xPos] = 0xFF;
    // uint8_t bit = (1 << (7 - xBitPos));

    // printf("For X: %d Y:%d | Binary: "BYTE_TO_BINARY_PATTERN"\n", x, y, BYTE_TO_BINARY(imageBuffer[y][xPos]));
    // printf("For X: %d Y:%d | Binary: "BYTE_TO_BINARY_PATTERN"\n", x, y, BYTE_TO_BINARY(bit));
    
}

void drawLineX(uint16_t startPosX, uint16_t length, uint16_t startPosY, bool color)
{
    if(startPosX > 199 || startPosY > 199)
    {
        printf("%s: out of bound for X:%d Y:%d\n", __func__, startPosX, startPosY);
        return;
    }

    for(int i = 0; i < length; i++)
    {
        if(startPosX + i > 199)
        {
            printf("%s: Warning: Line write out of bound for X:%d Y:%d at rel length: %d\n", __func__, startPosX, startPosY, startPosX + i);
        }
        drawPixel(startPosX + i, startPosY, color);
    }
}

void drawLineY(uint16_t startPosX, uint16_t length, uint16_t startPosY, bool color)
{
    if(startPosX > 199 || startPosY > 199)
    {
        printf("%s: out of bound for X:%d Y:%d\n", __func__, startPosX, startPosY);
        return;
    }

    for(int i = 0; i < length; i++)
    {
        if(startPosY + i > 199)
        {
            printf("%s: Warning: Line write out of bound for X:%d Y:%d at rel length: %d\n", __func__, startPosX, startPosY, startPosX + i);
        }
        drawPixel(startPosX, startPosY + i, color);
    }
}

//Write a single letter on the LCD
void writeLetter(uint16_t posX, uint16_t posY, bool color, char inChar)
{
    if(posX > 199 || posY > 199)
    {
        printf("%s: out of bound for X:%d Y:%d\n", __func__, posX, posY);
        return;
    }


    //Return if invalid character entered
    if(inChar < ' ' || inChar > '~')
        return;

    //Offset the input if spce is detected
    if(inChar == ' ')
        inChar = '~' + 1;

    //Offset character to index 0
    inChar -= '!';

    //Loop counters
    int i = 0;
    int j = 0;
    //Start and end index of the array
    int startFont = inChar * 11;
    int endFont = (inChar * 11) + 10;
    // setCursorXY(posX, posY);

    //Send command
    for(i = startFont; i <= endFont; i++){
        //Write X axis
        for(j = 7; j >= 0; j--){
            //Write Y axis
            uint16_t temp = ((lucidaConsole_8ptBitmaps_narrow[i] >> j) & 0x01);
            if(temp){
                // spi16bytes(color);
                drawPixel(posX, posY, DISP_BLACK);
            } else {
                // spi16bytes(BLACK);
                drawPixel(posX, posY, DISP_WHITE);
            }
            posX++;
        }
        posX -= 8;

        //Set the new cursor position
        // setCursorXY(posX, ++posY);
        posY++;

    }
}

//Write a string of characters on the LCD
void writeString(uint16_t posX, uint16_t posY, bool color, char *inString)
{
    //If the string still exists, write it to the LCD
    while(*inString){
        writeLetter(posX, posY, color, *inString++);
        //Move X to the right by the character width
        posX += 7;
    }
}

void updateDisplay(void)
{
    SetMemoryArea(0, 0, 200 - 1, 200 - 1);
    SetMemoryPointer(0, 0);
    SendCommand(WRITE_RAM);
    for(int i = 0; i < 200; i++)
    {
        for(int j = 0; j < 200 / 8; j++)
        {
            // printf("Send: %d %d\n", i, j);
            SendData(imageBuffer[i][j]);
            // printf("SENT FUNC X: %d Y:%d \t Binary: "BYTE_TO_BINARY_PATTERN"\n", i, j, BYTE_TO_BINARY(imageBuffer[i][j]));
        }
    }
    DisplayFrame();
}


int main(void)
{
    if (wiringPiSetupGpio () == -1) {
        printf("Wiring pi fault\n");
        return 1 ;
    }

    Init(lut_full_update);

    // ClearFrameMemory(0x00);   // bit set = white, bit reset = black
    // DisplayFrame();
    ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    DisplayFrame();

    for(int i = 0; i < 200; i++)
    {
        for(int j = 0; j < 200 / 8; j++)
        {
            imageBuffer[i][j] = 0xFF;
        }
    }
    

    // for(long i = 0; i < 2500; i++)
    // {
    //     if(i < 20000)
    //         imageBuf[i] = 0x0f;
    //     else
    //         imageBuf[i] = 0x00;
    // }

    // SetMemoryArea(0, 0, 200 - 1, 200 - 1);
    // SetMemoryPointer(0, 0);
    // SendCommand(WRITE_RAM);
    // // /* send the color data */
    // for (int i = 0; i < 200 / 8 * 200; i++) {
    //     SendData(imageBuf[i]);
    // }    
    
    // for (int i = 0; i < 40000; i++) {
    //     SendData(imageBuf[i]);
    // }
    // DisplayFrame();

    // SetFrameMemory(imageBuf, 0, 0, 199, 199);
    // DisplayFrame();

//   paint.SetRotate(ROTATE_0);
//   paint.SetWidth(200);
//   paint.SetHeight(24);

    for(int i = 0; i < 200; i++)
    {
        drawPixel(i, i, DISP_BLACK);
        // printf("Draw for: %d\n", i);
    }
    // for (int i = 0; i < 200 / 8 * 200; i++) {
    //     SendData(imageBuf[i]);
    // } 
    
    drawLineX(100, 50, 10, DISP_BLACK);
    updateDisplay();

    drawLineY(10, 100, 100, DISP_BLACK);
    updateDisplay();

    writeString(20, 20, DISP_BLACK, "Prayag");
    updateDisplay();

    return 0;
}