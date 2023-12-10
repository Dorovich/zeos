/*
 * io.c - 
 */

#include <io.h>

#include <types.h>

/**************/
/** Screen  ***/
/**************/

#define NUM_COLUMNS 80
#define NUM_ROWS    25

struct pointer_struct point;

Byte x, y=19;

/* Read a byte from 'port' */
Byte inb (unsigned short port)
{
  Byte v;

  __asm__ __volatile__ ("inb %w1,%0":"=a" (v):"Nd" (port));
  return v;
}

void printc(char c)
{
     __asm__ __volatile__ ( "movb %0, %%al; outb $0xe9" ::"a"(c)); /* Magic BOCHS debug: writes 'c' to port 0xe9 */
  if (c=='\n')
  {
    point.x = 0;
    point.y=(point.y+1)%NUM_ROWS;
  }
  else
  {
      Word ch = (Word) (c & 0x00FF) | (point.fg<<8 & 0x0F00) | (point.bg<<12 & 0x7000);
      Word *screen = (Word *)0xb8000;
      screen[(point.y * NUM_COLUMNS + point.x)] = ch;
    if (++point.x >= NUM_COLUMNS)
    {
      point.x = 0;
      point.y=(point.y+1)%NUM_ROWS;
    }
  }
}

void printc_color(char c, char l)
{
     __asm__ __volatile__ ( "movb %0, %%al; outb $0xe9" ::"a"(c)); /* Magic BOCHS debug: writes 'c' to port 0xe9 */
  if (c=='\n')
  {
    point.x = 0;
    point.y=(point.y+1)%NUM_ROWS;
  }
  else
  {
      Word ch = (Word) (c & 0x00FF) | (l & 0xFF00);
      Word *screen = (Word *)0xb8000;
      screen[(point.y * NUM_COLUMNS + point.x)] = ch;
    if (++point.x >= NUM_COLUMNS)
    {
      point.x = 0;
      point.y=(point.y+1)%NUM_ROWS;
    }
  }
}

void printc_xy(Byte mx, Byte my, char c)
{
  Byte cx, cy;
  cx=point.x;
  cy=point.y;
  point.x=mx;
  point.y=my;
  printc(c);
  point.x=cx;
  point.y=cy;
}

void printk(char *string)
{
  int i;
  for (i = 0; string[i]; i++)
    printc(string[i]);
}

int point_to (int x, int y, int fg, int bg) {

    /*
      TODO: cambiar el uso de 'x' e 'y' en este archivo por 'point.x' y 'point.y'
    */
    
    if (x < 0 || x > NUM_COLUMNS || y < 0 || y > NUM_ROWS) return -1;
    point.x = x;
    point.y = y;
    point.fg = fg;
    point.bg = bg;
    return 0;
}

int set_screen (char *s) {
    int cx = point.x, cy = point.y;
    point.x = 0; point.y = 0;
    for (int i=0; i<NUM_ROWS; ++i)
        for (int j=0; j<NUM_COLUMNS; ++j)
            printc(s != NULL ? s[i*NUM_ROWS+j] : ' ');
    point.x = cx; point.y = cy;
    return 0;
}
