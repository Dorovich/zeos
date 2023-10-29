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
  
  /* int n = 12345; */
  /* itoa(n, buf); */
  /* itoa(n, buf); */
  /* int written = write(1, buf, strlen(buf)); */
  /* itoa(written, buf); */
  /* write(1, buf, strlen(buf)); */
    
  /* int n2 = 67890; */
  /* itoa(n2, buf); */
  /* written = write(1, buf, strlen(buf)); */
  /* itoa(written, buf); */
  /* write(1, buf, strlen(buf)); */
  /* itoa(n2, buf); */
  /* written = write(1, buf, strlen(buf)); */
  /* itoa(written, buf); */
  /* write(1, buf, strlen(buf)); */

  write(-1, buf, strlen(buf));
  /* perror(); */
  
  /* buf[0] = 'A'; */
  /* buf[1] = ' '; */
  /* buf[2] = '\0'; */

  int pid = fork();  
  itoa(pid, buf);
    
  while(1) {
      for (unsigned int i=0; i<500000; ++i);
      /* write(1, buf, strlen(buf)); */
      write(1, buf, strlen(buf));
  }
}
