/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>
#include <utils.h>
#include <io.h>
#include <mm.h>
#include <mm_address.h>
#include <sched.h>

#define LECTURA 0
#define ESCRIPTURA 1

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{
}

int sys_write(int fd, char *buffer, int size)
{
// Fer una copia de *buffer a una variable (es mem de usuari)
    int valido = check_fd(fd, ESCRIPTURA);
    if (valido < 0) return valido;
    if (buffer == NULL) return -1;
    if (size < 0) return -1;

    if (fd == 1) return sys_write_console(buffer, size);

    return 0;
}

int sys_gettime()
{
    return zeos_ticks;
}
