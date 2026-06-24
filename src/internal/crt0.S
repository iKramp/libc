.text

.extern saved_rax
.extern saved_rbx
.extern saved_rcx
.extern saved_rdx
.extern saved_rsi
.extern saved_rdi
.extern saved_rsp
.extern saved_rbp
.extern saved_r8
.extern saved_r;
.extern saved_r10
.extern saved_r11
.extern saved_r12
.extern saved_r13
.extern saved_r14
.extern saved_r15
.extern _libc_start

.global _start


_start:
mov [rel saved_rax], rax
mov [rel saved_rbx], rbx
mov [rel saved_rcx], rcx
mov [rel saved_rdx], rdx
mov [rel saved_rsi], rsi
mov [rel saved_rdi], rdi
mov [rel saved_rsp], rsp
mov [rel saved_rbp], rbp
mov [rel saved_r8], r8
mov [rel saved_r9], r9
mov [rel saved_r10], r10
mov [rel saved_r11], r11
mov [rel saved_r12], r12
mov [rel saved_r13], r13
mov [rel saved_r14], r14
mov [rel saved_r15], r15
call _libc_start
