/* src/blibc/special/start.S */
    .text
    .global _start
    .type _start, @function
_start:
    mov %rsp, %rdi
    call __blibc_start_c
    mov %eax, %edi
    mov $60, %rax      /* SYS_exit */
    syscall
    /* shouldnt return */
    hlt
