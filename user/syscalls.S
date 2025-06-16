.global exit
.global return_exit
.global cons_write
.global start
.global waitpid
.global kill
.global chprio
.global getprio
.global getpid
.global pcreate
.global pdelete
.global pcount
.global psend
.global preceive
.global preset
.global wait_clock

.macro SAVE_REGS
    pushf
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esi
    pushl %edi
.endm

.macro RESTORE_REGS
    popl %edi
    popl %esi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    popf
.endm

.macro SAVE_REGS_WITHOUT_EAX
    pushf
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esi
    pushl %edi
.endm

.macro RESTORE_REGS_WITHOUT_EAX
    popl %edi
    popl %esi
    popl %edx
    popl %ecx
    popl %ebx
    popf
.endm

exit:
    SAVE_REGS
    movl $1, %eax
    movl 4+28(%esp), %ebx
    int $0x49
    RESTORE_REGS
    ret
    
return_exit:
    SAVE_REGS
    movl %eax, %ebx
    movl $1, %eax
    int $0x49
    RESTORE_REGS
    ret

cons_write:
    SAVE_REGS
    movl $2, %eax
    movl 4+28(%esp), %ebx
    movl 8+28(%esp), %ecx
    int $0x49
    RESTORE_REGS
    ret

start:
    SAVE_REGS_WITHOUT_EAX
    movl $3, %eax
    movl 4+24(%esp), %ebx
    movl 8+24(%esp), %ecx
    movl 12+24(%esp), %edx
    movl 16+24(%esp), %esi
    movl 20+24(%esp), %edi
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

waitpid:
    SAVE_REGS_WITHOUT_EAX
    movl $4, %eax
    movl 4+24(%esp), %ebx
    movl 8+24(%esp), %ecx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

kill:
    SAVE_REGS_WITHOUT_EAX
    movl $5, %eax
    movl 4+24(%esp), %ebx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

chprio:
    SAVE_REGS_WITHOUT_EAX
    movl $6, %eax
    movl 4+24(%esp), %ebx
    movl 8+24(%esp), %ecx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

getprio:
    SAVE_REGS_WITHOUT_EAX
    movl $7, %eax
    movl 4+24(%esp), %ebx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

getpid:
    SAVE_REGS_WITHOUT_EAX
    movl $8, %eax
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

pcreate:
    SAVE_REGS_WITHOUT_EAX
    movl $9, %eax
    movl 4+24(%esp), %ebx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

pdelete:
    SAVE_REGS_WITHOUT_EAX
    movl $10, %eax
    movl 4+24(%esp), %ebx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

pcount:
    SAVE_REGS_WITHOUT_EAX
    movl $11, %eax
    movl 4+24(%esp), %ebx
    movl 8+24(%esp), %ecx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

psend:
    SAVE_REGS_WITHOUT_EAX
    movl $12, %eax
    movl 4+24(%esp), %ebx
    movl 8+24(%esp), %ecx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

preceive:
    SAVE_REGS_WITHOUT_EAX
    movl $13, %eax
    movl 4+24(%esp), %ebx
    movl 8+24(%esp), %ecx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

preset:
    SAVE_REGS_WITHOUT_EAX
    movl $14, %eax
    movl 4+24(%esp), %ebx
    int $0x49
    RESTORE_REGS_WITHOUT_EAX
    ret

wait_clock:
    SAVE_REGS
    movl $15, %eax
    movl 4+28(%esp), %ebx
    int $0x49
    RESTORE_REGS
    ret
