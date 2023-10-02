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

  char msg[256] = "Hola xd.";
  write(1, msg, strlen(msg));

  int t;
  while(1) {
    for(int i=0; i<1000; ++i);
    t = gettime();
    itoa(t, msg);
    write(1, msg, strlen(msg));
  }
}
