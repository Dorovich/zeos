#include <libc.h>

char buff[24];

int pid;

int add(int a, int b) { return a + b; }
int addAsm(int a, int b);

int __attribute__ ((__section__(".text.main")))
  main(void)
{
  /* int t = gettime(); */
  /* for(int i = 0; i < 10000000; ++i); */
  /* t = gettime(); */
  
  char buf[256];

  /* write(-1, buf, strlen(buf)); */
  /* perror(); */

  int pid = fork();  
  itoa(pid, buf);
    
  while(1) {
      for (unsigned int i=0; i<500000; ++i);
      write(1, buf, strlen(buf));
  }
}
