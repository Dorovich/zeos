#include <libc.h>

char buff[24];

int pid;

int add(int a, int b) { return a + b; }
int addAsm(int a, int b);

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  /* int r = add(0x69, 0b010101001); */
  /* int r = addAsm(32, 37); */

  char msg[9] = "Hola xd.";
  write(1, msg, 9);

  char* p = 0;
  *p = 'x';

  int t = gettime();

  for(int i = 0; i < 10000000; ++i);

  t = gettime();

  while(1);
}
