#include <libc.h>
#include <colors.h>

#define NULL (void *)0

struct sem_t *semaforo;
char letter;
/* int counter; */

void error() {
    char str[64] = "(ERROR!)";
    write(1, str, sizeof(str));
}

void message() {
    char str[64] = "(HOLA!)";
    write(1, str, sizeof(str));
}

void writer (void *ingored) {
    if (fork() == 0) {
        gotoXY(9, 20);
        changeColor(C_LIGHTGREEN, C_BLUE);
        message();
        exit();
    }

    int ret = waitKey(&letter, 60);
    if (ret<0) semDestroy(semaforo);
    else semSignal(semaforo);
}

void reader (void *ignored) {
    int ret = semWait(semaforo);
    if (ret<0) {
        error();
        exit();
    }
    write(1, &letter, sizeof(char));
    semDestroy(semaforo);
}

int __attribute__ ((__section__(".text.main")))
main(void)
{
    gotoXY(0, 0);
    changeColor(C_BLACK, C_BLACK);
    clrscr((char*)0);
    changeColor(C_YELLOW, C_RED);
    char msg[64] = "   Codigo de usuario ejecutandose:\n";
    write(1, msg, strlen(msg));
    gotoXY(0, 10);
    
    int ret;
    
    letter = 'x';
    semaforo = semCreate(0);
    ret = threadCreateWithStack(reader, 1, NULL);
    if (ret<0) error();
    ret = threadCreateWithStack(writer, 1, NULL);
    if (ret<0) error();

    char *cosa = memRegGet(3);
    for (int i = 0; i<10; ++i) {
        cosa[i] = 'a'+i;
    }
    cosa[10] = '\0';
    write(1, cosa, strlen(cosa));
    memRegDel(cosa);

    while(1);
}
