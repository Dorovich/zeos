/*
 * io.h - Definició de l'entrada/sortida per pantalla en mode sistema
 */

#ifndef __IO_H__
#define __IO_H__

#include <types.h>

struct pointer_struct {
	int x, y;
	int fg, bg;
};

extern struct pointer_struct point;

/** Screen functions **/
/**********************/

Byte inb (unsigned short port);
void printc(char c);
void printc_xy(Byte x, Byte y, char c);
void printk(char *string);
int point_to (int x, int y, int fg, int bg);
int set_screen(char *s);

extern int zeos_ticks;

#endif  /* __IO_H__ */
