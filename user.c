#include <libc.h>

char buff[24];

int pid;

int add(int a, int b) { return a + b; }
int addAsm(int a, int b);

int __attribute__ ((__section__(".text.main")))
main(void)
{
    char c;
    int err = waitKey(&c, 5);
    if (!err) write(1, &c, 1);
    while(1);
}
