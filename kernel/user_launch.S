    .globl user_launch

user_launch:
    pushl %eax 
    movw $0x4b, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    popl %eax
    iret
