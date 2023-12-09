#include <libc.h>
#include <colors.h>

char buff[24];

int pid;

int add(int a, int b) { return a + b; }
int addAsm(int a, int b);

void hello_there (void *n) {
    char str[16];
    itoa(*(int *)n, str);
    write(1, str, sizeof(str));
}

void error() {
    char str[64] = "(ERROR!)";
    write(1, str, sizeof(str));    
}

int __attribute__ ((__section__(".text.main")))
main(void)
{
    gotoXY(0, 10);
    changeColor(C_BLACK, C_BLACK);
    clrscr((char*)0);
    changeColor(C_YELLOW, C_RED);
    char msg[64] = "   Codigo de usuario ejecutandose:\n";
    write(1, msg, strlen(msg));
    
    int ret;

    int n1 = 10, n2 = 20, n3 = 30;
    ret = threadCreateWithStack(hello_there, 1, (void *)&n1);
    if (ret<0) error();
    /* ret = threadCreateWithStack(hello_there, 2, (void *)&n2); */
    /* if (ret<0) error(); */
    /* ret = threadCreateWithStack(hello_there, 1, (void *)&n3); */
    /* if (ret<0) error(); */

    int n4 = 40;
    char str[16];
    itoa(n4, str);
    write(1, str, sizeof(str));
    
    /* char c, err; */
    while(1) {
        /* err = waitKey(&c, 30); */
        /* if (err >= 0) { */
        /*     write(1, &c, sizeof(char)); */
        /* } else { */
        /*     char alert[30] = "Error: TIMEOUT.\n"; */
        /*     write(1, alert, strlen(alert)); */
        /* } */
    }
}
