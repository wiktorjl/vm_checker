#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/io.h>

static sigjmp_buf jump_buffer;

void signal_handler(int sig) {
    siglongjmp(jump_buffer, 1);
}

int check_hypervisor_port() {
    int is_vm = 0;
    unsigned char result = 0;

    signal(SIGSEGV, signal_handler);

    if (sigsetjmp(jump_buffer, 1) == 0) {
        if (ioperm(0x5658, 1, 1) == 0) { // Grant access to the port
            __asm__ (
                "push %%rdx\n"
                "push %%rcx\n"
                "push %%rbx\n"
                "mov $0x564D5868, %%eax\n"  // 'VMXh'
                "xor %%ebx, %%ebx\n"
                "mov $10, %%ecx\n"
                "mov $0x5658, %%edx\n"     // 'VX'
                "in %%dx, %%eax\n"
                "cmp $0x564D5868, %%ebx\n" // 'VMXh'
                "setz %0\n"
                "pop %%rbx\n"
                "pop %%rcx\n"
                "pop %%rdx\n"
                : "=r" (result)
                :
                : "rax", "rbx", "rcx", "rdx"
            );
            is_vm = result != 0;
            ioperm(0x5658, 1, 0); // Revoke access to the port
        }
    } else {
        // Caught a SIGSEGV
        is_vm = 0;
    }

    return is_vm;
}

int main() {
    if (check_hypervisor_port())
        printf("VMware hypervisor detected.\n");
    else
        printf("No VMware hypervisor detected.\n");
    return 0;
}
