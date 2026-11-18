#ifndef LIBC_SIGNAL_H
#define LIBC_SIGNAL_H

typedef int sig_atomic_t;

#define SIG_DFL ((void (*)(int))1)
#define SIG_ERR ((void (*)(int))2)
#define SIG_IGN ((void (*)(int))3)

#define SIGABRT 6
#define SIGFPE 8
#define SIGILL 4
#define SIGINT 2
#define SIGSEGV 11
#define SIGTERM 15

void (*signal(int sig, void (*func)(int)))(int); 

#endif
