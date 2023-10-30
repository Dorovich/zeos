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

    struct stats s;
    
    /* int pid = fork(); */
    
    while(1) {
        for (unsigned int i=0; i<500000; ++i);
        get_stats(getpid(), &s);
        itoa(&s.user_ticks, buf);
        write(1, buf, strlen(buf));
        buf[0] = ' ';
        buf[1] = '\0';
        write(1, buf, strlen(buf));
    }
}
