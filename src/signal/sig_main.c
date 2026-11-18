#include "signal.h"
#include "specific/null.h"
#include "syscalls/namespace.h"
#include "syscalls/proc.h"
#include <stdint.h>

extern int pid;

struct RecvNamespaceSignalData {
    uint32_t pid;
    uint64_t namespace_id;
};

void recv_namespace_ignore(uint64_t sig_num, struct RecvNamespaceSignalData *data) {
    if (sig_num != SIG_RECV_NAMESPACE) {
        return;
    }

    _rmnamespace(data->namespace_id);
}

void ignore_signal() {
    return;
}

static void (*handlers[128])(int) = { SIG_DFL };

void (*signal(int sig, void (*func)(int)))(int) {
    if (func == SIG_DFL) {
        int res = _remove_sig_handler(sig);
        if (res < 0) {
            return SIG_ERR;
        }
        void (*old_handler)(int) = handlers[sig];
        handlers[sig] = SIG_DFL;
        return old_handler;
    }
    if (func == SIG_IGN) {
        int res = _set_sig_handler(sig, ignore_signal);
        if (res < 0) {
            return SIG_ERR;
        }
        void (*old_handler)(int) = handlers[sig];
        handlers[sig] = SIG_IGN;
        return old_handler;
    }

    void (*old_handler)(int) = handlers[sig];
    int res = _set_sig_handler(sig, func);
    if (res < 0) {
        return SIG_ERR;
    }
    handlers[sig] = func;
    return old_handler;
}

int raise(int sig) {
    if (pid < 0) {
        return -1;
    }
    return _send_signal(pid, sig, 0, NULL);
}
