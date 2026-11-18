#include "signal.h"

static void (*handlers[128])(int) = { SIG_DFL };

void (*signal(int sig, void (*func)(int)))(int) {
    // This is a stub implementation of the signal function.
    // In a real implementation, you would store the signal handler
    // and return the previous handler.
    return SIG_DFL; // Return default handler for simplicity
}
