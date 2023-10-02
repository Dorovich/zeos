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
    //char msg[256] = "Hola xd.";
    //write(1, msg, strlen(msg));


  //int t = 0;
  while(1) {
    //t = gettime();
    for (int i = 3000; i < 100000; i = i + 1) {
      char msg[256];
      itoa(i, msg);
      write(1, msg, strlen(msg));
      char* c = " ";
      write(1, c, strlen(c));
    }
  }
}
