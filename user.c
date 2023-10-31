#include <libc.h>

char buff[24];

int pid;

int add(int a, int b) { return a + b; }
int addAsm(int a, int b);

int __attribute__ ((__section__(".text.main")))
main(void)
{
    char buf[1024] = "Hola que tal, este es un mensaje superlargo que he hecho solo para que exceda el tamanyo del buffer de escritura del sistema operativo y comprovar que funciona como deberia. No te preocupes, si todo funciona bien no deberia pasar nada raro, pero porsiacaso voy a comprobarlo igualmente... Lo que deberia hacer el sistema operativo es copiar y ecribir este mensaje por fragmentos, ya que hay tantos caracteres que excede el valor de la variable WRITE_MAX (256) que tiene definido. Este mensaje tiene 520 caracteres jaja.\n";

    int written = write(1, buf, strlen(buf));
    itoa(written, buf);
    write(1, buf, strlen(buf));
    
    /* perror(); */
    
    /* int t = gettime(); */
    /* for(int i = 0; i < 10000000; ++i); */
    /* t = gettime(); */

    if (fork() == 0) {
        char b[24] = "Hola soy el hijo :)\n\0";
        write(1, b, strlen(b));
        exit();
    } else {
        char b[24] = "Hola soy el padre :)\n\0";
        write(1, b, strlen(b));
    }
    
    while(1) {
        /* struct stats s; */
        
        /* for (unsigned int i=0; i<1000000; ++i); */
        /* get_stats(getpid(), &s); */

        /* buf[0] = '['; */
        /* buf[1] = '\0'; */
        /* write(1, buf, strlen(buf)); */
        
        /* itoa(s.user_ticks, buf); */
        /* write(1, buf, strlen(buf)); */
        /* buf[0] = ','; */
        /* buf[1] = ' '; */
        /* buf[2] = '\0'; */
        /* write(1, buf, strlen(buf)); */
        
        /* itoa(s.system_ticks, buf); */
        /* write(1, buf, strlen(buf)); */
        /* buf[0] = ','; */
        /* buf[1] = ' '; */
        /* buf[2] = '\0'; */
        /* write(1, buf, strlen(buf)); */
        
        /* itoa(s.ready_ticks, buf); */
        /* write(1, buf, strlen(buf)); */
        /* buf[0] = ','; */
        /* buf[1] = ' '; */
        /* buf[2] = '\0'; */
        /* write(1, buf, strlen(buf)); */
        
        /* itoa(s.elapsed_total_ticks, buf); */
        /* write(1, buf, strlen(buf)); */
        
        /* buf[0] = ']'; */
        /* buf[1] = ' '; */
        /* buf[2] = '\0'; */
        /* write(1, buf, strlen(buf)); */
    }
}
