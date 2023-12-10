/*
 * stubs_printf.c
 *
 *  Created on: 	01/dic/2013
 *  Last Update on: 28/apr/2014
 *  Version:		Beta 0.5
 *  Author: 		Jek
 *
 */

#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include "stm32f2xx_usart.h"
#include "main.h"

#if (!defined (PRINTF_USART1) && !defined (PRINTF_USART2) && !defined (PRINTF_USART3)  && !defined (PRINTF_USART6) && !defined (PRINTF_USB) && !defined (PRINTF_OFF))
 #error "Please DEFINE one of printf target :"
 #error "   -> PRINTF_USART1"
 #error "   -> PRINTF_USART2"
 #error "   -> PRINTF_USART3"
 #error "   -> PRINTF_USART5"
 #error "   -> PRINTF_USART6"
// #error "   -> PRINTF_USB"
 #error "   -> PRINTF_OFF (if not use)"
 #error "in to main.h file please."

#else

#ifndef PRINTF_OFF

#ifdef PRINTF_USART1
#define USART_SR 	 (USART1->SR)
#define USART_TXdata (USART1->DR)
#define USART_RXdata (USART1->DR)
#endif

#ifdef PRINTF_USART2
#define USART_SR 	 (USART2->SR)
#define USART_TXdata (USART2->DR)
#define USART_RXdata (USART2->DR)
#endif

#ifdef PRINTF_USART3
#define USART_SR 	 (USART3->SR)
#define USART_TXdata (USART3->DR)
#define USART_RXdata (USART3->DR)
#endif

#ifdef PRINTF_USART5
#define USART_SR 	 (USART5->SR)
#define USART_TXdata (USART5->DR)
#define USART_RXdata (USART5->DR)
#endif

#ifdef PRINTF_USART6
#define USART_SR 	 (USART6->SR)
#define USART_TXdata (USART6->DR)
#define USART_RXdata (USART6->DR)
#endif

#ifdef PRINTF_USB
#include "usbd_cdc_vcp.h"
#endif

#undef errno
extern int errno;

/*
 environ
 A pointer to a list of environment variables and their values.
 For a minimal environment, this empty list is adequate:
 */
char *__env[1] = { 0 };
char **environ = __env;

int _write(int file, char *ptr, int len);

void _exit(int status) {
    _write(1, "exit", 4);
    while (1) {
        ;
    }
}

int _close(int file) {
    return -1;
}
/*
 execve
 Transfer control to a new process. Minimal implementation (for a system without processes):
 */
int _execve(char *name, char **argv, char **env) {
    errno = ENOMEM;
    return -1;
}
/*
 fork
 Create a new process. Minimal implementation (for a system without processes):
 */

int _fork() {
    errno = EAGAIN;
    return -1;
}
/*
 fstat
 Status of an open file. For consistency with other minimal implementations in these examples,
 all files are regarded as character special devices.
 The `sys/stat.h' header file required is distributed in the `include' subdirectory for this C library.
 */
int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/*
 getpid
 Process-ID; this is sometimes used to generate strings unlikely to conflict with other processes. Minimal implementation, for a system without processes:
 */

int _getpid() {
    return 1;
}

/*
 isatty
 Query whether output stream is a terminal. For consistency with the other minimal implementations,
 */
int _isatty(int file) {
    switch (file){
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        //errno = ENOTTY;
        errno = EBADF;
        return 0;
    }
}


/*
 kill
 Send a signal. Minimal implementation:
 */
int _kill(int pid, int sig) {
    errno = EINVAL;
    return (-1);
}

/*
 link
 Establish a new name for an existing file. Minimal implementation:
 */

int _link(char *old, char *new) {
    errno = EMLINK;
    return -1;
}

/*
 lseek
 Set position in a file. Minimal implementation:
 */
int _lseek(int file, int ptr, int dir) {
    return 0;
}

/*
 sbrk
 Increase program data space.
 Malloc and related functions depend on this
 */
caddr_t _sbrk(int incr) {

    extern char _ebss; // Defined by the linker
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_ebss;
    }
    prev_heap_end = heap_end;

char * stack = (char*) __get_MSP();
     if (heap_end + incr >  stack)
     {
         _write (STDERR_FILENO, "Heap and stack collision\n", 25);
         errno = ENOMEM;
         return  (caddr_t) -1;
         //abort ();
     }

    heap_end += incr;
    return (caddr_t) prev_heap_end;
}

/*
 read
 Read a character to a file. `libc' subroutines will use this system routine for input from all files, including stdin
 Returns -1 on error or blocks until the number of characters have been read.
 */


int _read(int file, char *ptr, int len) {
    int n;
    int num = 0;
    switch (file) {
    case STDIN_FILENO:
#ifdef PRINTF_USB
    	VCP_DataRX(ptr,len);
#else
        for (n = 0; n < len; n++) {
            while ((USART_SR & USART_FLAG_RXNE) == (uint16_t)RESET) {}
            char c = (char)(USART_RXdata & (uint16_t)0x01FF);
            *ptr++ = c;
            num++;
        }
#endif
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return num;
}

/*
 stat
 Status of a file (by name). Minimal implementation:
 int    _EXFUN(stat,( const char *__path, struct stat *__sbuf ));
 */

int _stat(const char *filepath, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/*
 times
 Timing information for current process. Minimal implementation:
 */

clock_t _times(struct tms *buf) {
    return -1;
}

/*
 unlink
 Remove a file's directory entry. Minimal implementation:
 */
int _unlink(char *name) {
    errno = ENOENT;
    return -1;
}

/*
 wait
 Wait for a child process. Minimal implementation:
 */
int _wait(int *status) {
    errno = ECHILD;
    return -1;
}

/*
 write
 Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
 Returns -1 on error or number of bytes sent
 */
int _write(int file, char *ptr, int len) {
    int n;
    switch (file) {
    case STDOUT_FILENO: /*stdout*/
#ifdef PRINTF_USB
    	VCP_DataTx(ptr,len);
#else
        for (n = 0; n < len; n++) {
            while ((USART_SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART_TXdata = (*ptr++ & (uint16_t)0x01FF);
        }
#endif
        break;
    case STDERR_FILENO: /* stderr */
#ifdef PRINTF_USB
    	VCP_DataTx(ptr,len);
#else
        for (n = 0; n < len; n++) {
            while ((USART_SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART_TXdata = (*ptr++ & (uint16_t)0x01FF);
        }
#endif
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}
#endif // PRINTF_OFF
#endif // ERROR not declared
