section .text
global _start

_start:
    push ebx
    xor ebx, ebx
    mov eax, 1
    mov ebx, 1
    test ebx, ebx
    setz al
    pop ebx
    mov ebx, 0
    mov eax, 1 
    int 0x80
