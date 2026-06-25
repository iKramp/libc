void __stack_chk_fail(void) {
    while(1);  // Infinite loop or handle error
}

// Some architectures also need this:
void *__stack_chk_guard = 0;
