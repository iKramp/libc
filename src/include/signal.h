#ifndef LIBC_SIGNAL_H
#define LIBC_SIGNAL_H

typedef int sig_atomic_t;

#define SIG_DFL ((void (*)(int))1)
#define SIG_ERR ((void (*)(int))2)
#define SIG_IGN ((void (*)(int))3)


#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGILL 4
#define SIGTRAP 5
#define SIGABRT 6
#define SIGBUS 7
#define SIGFPE 8
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGPIPE 13
#define SIGALRM 14
#define SIGTERM 15
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGTTIN 21
#define SIGTTOU 22
#define SIGURG 23
#define SIGXCPU 24
#define SIGXFSZ 25
#define SIGVTALRM 26
#define SIGPROF 27
#define SIGWINCH 28
#define SIGIO 29
#define SIGPWR 30
#define SIGSYS 31

#define SIG_RECV_NAMESPACE 64

void (*signal(int sig, void (*func)(int)))(int); 
int raise(int sig);

#endif
