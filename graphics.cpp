/*
 * graphic.c
 *
 *  Created on: 4 lip 2015
 *      Author: Piter
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include "sh1106.h"


/********************Definicje zmiennych********************/

uint8_t static bufor[lenght] = {};

int cursor_x, cursor_y;


/********************Czyszczenie bufora (wype³nienie zerami)********************/

void bufor_clear (void)
{
	memset(bufor, 0, (lenght));
}


/********************Rysowanie pixela********************/

void draw_bufor_pixel (int16_t x, int16_t y, uint8_t blackwhite)
{
	if( (x < 0) || (x >= SH1106_WIDTH) || (y < 0) || (y >= SH1106_HEIGHT) )
		return;

	if(blackwhite) bufor[x + (y / 8) * SH1106_WIDTH] |= (1<<(y%8)); //zapal pixel
	else bufor[x + (y / 8) * SH1106_WIDTH] &= ~(1<<(y%8));			 //zgaœ pixel
}


/********************Rysowanie lini********************/

void draw_bufor_line (int x0, int y0, int x1, int y1, uint8_t blackwhite)
{
	int steep = abs(y1 - y0) > abs(x1 - x0);
	if(steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if(x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int ystep, err = dx / 2;

	if(y0 < y1)	ystep = 1;
	else	ystep = -1;

	for(; x0 <= x1; x0++)
	{
		if(steep)	draw_bufor_pixel(y0, x0, blackwhite);
		else	draw_bufor_pixel(x0, y0, blackwhite);

		err -= dy;
		if(err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

void draw_bufor_vline (int8_t x, int8_t y1, int8_t y2, uint8_t blackwhite)
{
	if(y1 < y2)
	{
		for( int8_t i=y1; i<y2; i++)
		{
			draw_bufor_pixel(x,i,blackwhite);
		}
	}

	else
	{
		for( int8_t i=y1; i>y2; i--)
		{
			draw_bufor_pixel(x,i,blackwhite);
		}
	}
}

void draw_bufor_hline (int8_t x1, int8_t x2, int8_t y, uint8_t blackwhite)
{
	if(x1 < x2)
	{
		for( int8_t i=x1; i<x2; i++)
		{
			draw_bufor_pixel(i,y,blackwhite);
		}
	}

	else
	{
		for( int8_t i=x1; i>x2; i--)
		{
			draw_bufor_pixel(i,y,blackwhite);
		}
	}
}


/********************Rysowanie prostok¹ta********************/

void draw_bufor_rect (int x, int y, int width, int height, uint8_t blackwhite)
{
	for( int16_t i=x; i<x+width; i++ )	draw_bufor_line(i, y, i, y+height-1, blackwhite);
}


/********************Rysowanie bitmap********************/

void draw_bufor_bitmap (int16_t x, int16_t y, const uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t blackwhite)
{
	int i,j,byteWidth = (width + 7) / 8;

	for( j=0; j<height; j++)
	{
		for ( i=0; i<width; i++)
		{
			if( pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)) )
			{
				draw_bufor_pixel( x+i, y+j, blackwhite);
			}
		}
	}
}


/********************Rysowanie Fontów********************/

void draw_bufor_char (int x, int y,  char c, uint8_t blackwhite, uint8_t background, uint8_t size)
{
	if( (x >= SH1106_WIDTH) || (y >= SH1106_HEIGHT) || ((x + 6 * size - 1) < 0) || ((y + 8 * size - 1) < 0) ) return;

	uint8_t line;

	for ( int8_t i=0; i<6; i++ )
	{
		if(i == 5)	line = 0x0;
		else line = pgm_read_byte(font5x7 + (c * 5) + i);

		for ( int8_t j=0; j<8; j++ )
		{
			if(line & 0x1)
			{
				if(size == 1)	draw_bufor_pixel(x + i, y + j, blackwhite);
				else draw_bufor_rect(x + (i * size), y + (j * size), size, size, blackwhite);
			}
			else if(background != blackwhite)
			{
				if(size == 1)	draw_bufor_pixel(x + i, y + j, background);
				else draw_bufor_rect(x + i * size, y + j * size, size, size, background);
			}
		line >>= 1;
		}
	}
}


void draw_bufor_string (int x, int y, char * string, uint8_t blackwhite, uint8_t background, uint8_t size_text)
{
	cursor_x = x;
	cursor_y = y;

	while(*string)
	{
		draw_bufor_char(cursor_x, cursor_y, *string++, blackwhite, background, size_text);
		cursor_x += size_text * 6;
	}
}


void draw_bufor_int (int x, int y, int data, uint8_t blackwhite, uint8_t background, uint8_t size_txt)
{
	char buf [6];
	draw_bufor_string(x, y, itoa(data, buf, 10), blackwhite, background, size_txt);
}


void draw_bufor_long (int x, int y, long int data, uint8_t blackwhite, uint8_t background, uint8_t size_txt)
{
	char buf [11];
	draw_bufor_string(x, y, ltoa(data, buf, 10), blackwhite, background, size_txt);
}










































