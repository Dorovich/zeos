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

  char msg[64] = "Hola xd.";
  write(1, msg, strlen(msg));

  // perror();

  /* char* p = 0; */
  /* *p = 'x';  */ //0x114040

  /* int t = gettime(); */

  /* for(int i = 0; i < 10000000; ++i); */

  /* t = gettime(); */

  for(int i = 0; i < 10; ++i) {
      char num[256];
      itoa(i, num);
      write(1, num, strlen(num));
  }
  
  while(1);
}
