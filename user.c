#include <libc.h>
#include <colors.h>

char buff[24];

int pid;

int add(int a, int b) { return a + b; }
int addAsm(int a, int b);

// TODO hacer bandera españa

int __attribute__ ((__section__(".text.main")))
main(void)
{
    gotoXY(0, 10);
    changeColor(C_BLACK, C_BLACK);
    clrscr((char*)0);
    changeColor(C_YELLOW, C_RED);
    char msg[256] = "   Codigo de usuario ejecutandose:\n";
    write(1, msg, strlen(msg));
    
    char c, err;
    while(1) {
        err = waitKey(&c, 30);
        if (err >= 0) {
            write(1, &c, sizeof(char));
        } else {
            char alert[30] = "Error: TIMEOUT.\n";
            write(1, alert, strlen(alert));
        }
    }
}
