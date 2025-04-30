/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */ 
#define F_CPU 16000000UL

#include "LCD_GFX.h"
#include "ST7735.h"
#include "math.h"
#include <string.h>
#include <stdlib.h>


#define ROWS 128
#define COLUMNS 180

/******************************************************************************
* Local Functions
******************************************************************************/



/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
* @brief		Convert RGB888 value to RGB565 16-bit color data
* @note
*****************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255));
}

/**************************************************************************//**
* @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
* @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
* @note
*****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
	LCD_setAddr(x,y,x,y);
	SPI_ControllerTx_16bit(color);
}

/**************************************************************************//**
* @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
* @brief		Draw a character starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor){
	uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
	int i, j;
	if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
		for(i=0;i<5;i++){
			uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
			for(j=0;j<8;j++){
				if ((pixels>>j)&1==1){
					LCD_drawPixel(x+i,y+j,fColor);
				}
				else {
					LCD_drawPixel(x+i,y+j,bColor);
				}
			}
		}
	}
}


/******************************************************************************
* LAB 4 TO DO. COMPLETE THE FUNCTIONS BELOW.
* You are free to create and add any additional files, libraries, and/or
*  helper function. All code must be authentically yours.
******************************************************************************/

/**************************************************************************//**
* @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored circle of set radius at coordinates
* @note
*****************************************************************************/
void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
{      
    int leftEdge = x0 - radius;
    int rightEdge = x0 + radius;
    int topEdge = y0 - radius;
    int bottomEdge = y0 + radius;
    
    for (int x = leftEdge; x <= rightEdge; x++) {
        for (int y = topEdge; y <= bottomEdge; y++) {
            //Use circle equation to check if you should draw a pixel
            int dx = x - x0;
            int dy = y - y0;
            if (dx * dx + dy * dy <= radius * radius) {
                LCD_drawPixel(x, y, color);
            }
        }
    }

//    
//    //Need to send many addresses at once
//    for (int line = 0; line < 2 * radius; line++) {
//        int y = y0 - radius + line; 
//        int dy = y - y0;           
//
//        if ((int)fabs(dy) > radius) continue;
//
//        int dx = (int)sqrt(radius * radius - dy * dy);
//
//        int x1 = x0 - dx;
//        int x2 = x0 + dx;
//        
//        LCD_drawBlock(x1, y, x2, y, color);
//   }
//    
    
//    //Draw a square first
//    int x1 = x0 - radius;
//    int y1 = y0 - radius;
//    int x2 = x0 + radius;
//    int y2 = y2 + radius;
//    
//    LCD_drawBlock(x1,y1,x2,y2,color);
    
    
}


/**************************************************************************//**
* @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
* @brief		Draw a line from and to a point with a color
* @note
*****************************************************************************/
void LCD_drawLine(short x0, short y0, short x1, short y1, uint16_t c) {
    if (x0 > x1) {
        short temp = x1;
        x1 = x0;
        x0 = temp;
        temp = y1;
        y1 = y0;
        y0 = temp;
    }
   
    if (x0 == x1 && y0 == y1) {
        LCD_drawPixel(x0, y0, c);
        return;
    }
   
    if (x0 == x1) {
        if (y0 > y1) {
            short temp = y1; y1 = y0; y0 = temp;
        }
        for (short y = y0; y <= y1; y++) {
            LCD_drawPixel(x0, y, c);
        }
        return;
    }
   
    if (y0 == y1) {
        for (short x = x0; x <= x1; x++) {
            LCD_drawPixel(x, y0, c);
        }
        return;
    }
   
    if (abs(y1 - y0) < abs(x1 - x0)) {
        pltLineL(x0, y0, x1, y1, c);
    } else {
        pltLineH(x0, y0, x1, y1, c);
    }
}

void pltLineL(short x0, short y0, short x1, short y1, uint16_t c) {
    short dx = x1 - x0;
    short dy = abs(y1 - y0);
    short yi = (y1 > y0) ? 1 : -1;
    int D = (2 * dy) - dx;
    short y = y0;
   
    for (short x = x0; x <= x1; x++) {
        LCD_drawPixel(x, y, c);
        if (D > 0) {
            y += yi;
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
}

void pltLineH(short x0, short y0, short x1, short y1, uint16_t c) {
    short dx = abs(x1 - x0);
    short dy = y1 - y0;
    short xi = (x1 > x0) ? 1 : -1;
    int D = (2 * dx) - dy;
    short x = x0;
   
    for (short y = y0; y <= y1; y++) {
        LCD_drawPixel(x, y, c);
        if (D > 0) {
            x += xi;
            D -= 2 * dy;
        }
        D += 2 * dx;
    }
}


/**************************************************************************//**
* @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw a colored block at coordinates
* @note
*****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{   
    LCD_setAddr(x0,y0,x1,y1);

	for (int x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            SPI_ControllerTx_16bit(color);
        }
    }
}

/**************************************************************************//**
* @fn			void LCD_setScreen(uint16_t color)
* @brief		Draw the entire screen to a color
* @note
*****************************************************************************/
void LCD_setScreen(uint16_t color) 
{   
    LCD_setAddr(0,0,COLUMNS,ROWS);
    
	for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS; y++) {
            SPI_ControllerTx_16bit(color);
        }
    }
}

/**************************************************************************//**
* @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
* @brief		Draw a string starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
{
	int i = 0;
    while (str[i] != '\0') {
        LCD_drawChar(x + (i * 5),y, str[i], fg, bg);
        i++;
    }
    
}

