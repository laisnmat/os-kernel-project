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
.endm

exit:
    SAVE_REGS
    movl $1, %eax
    movl 4+20(%esp), %ebx
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
    movl 4+20(%esp), %ebx
    movl 8+20(%esp), %ecx
    int $0x49
    RESTORE_REGS
    ret

start:
    SAVE_REGS
    movl $3, %eax
    movl 4+20(%esp), %ebx
    movl 8+20(%esp), %ecx
    movl 12+20(%esp), %edx
    movl 16+20(%esp), %esi
    movl 20+20(%esp), %edi
    int $0x49
    RESTORE_REGS
    ret

waitpid:
    SAVE_REGS
    movl $4, %eax
    movl 4+20(%esp), %ebx
    movl 8+20(%esp), %ecx
    int $0x49
    RESTORE_REGS
    ret

kill:
    SAVE_REGS
    movl $5, %eax
    movl 4+20(%esp), %ebx
    int $0x49
    RESTORE_REGS
    ret

chprio:
    SAVE_REGS
    movl $6, %eax
    movl 4+20(%esp), %ebx
    movl 8+20(%esp), %ecx
    int $0x49
    RESTORE_REGS
    ret

getprio:
    SAVE_REGS
    movl $7, %eax
    movl 4+20(%esp), %ebx
    int $0x49
    RESTORE_REGS
    ret

getpid:
    SAVE_REGS
    movl $8, %eax
    int $0x49
    RESTORE_REGS
    ret

pcreate:
    SAVE_REGS
    movl $9, %eax
    movl 4+20(%esp), %ebx
    int $0x49
    RESTORE_REGS
    ret

pdelete:
    SAVE_REGS
    movl $10, %eax
    movl 4+20(%esp), %ebx
    int $0x49
    RESTORE_REGS
    ret

pcount:
    SAVE_REGS
    movl $11, %eax
    movl 4+20(%esp), %ebx
    movl 8+20(%esp), %ecx
    int $0x49
    RESTORE_REGS
    ret

psend:
    SAVE_REGS
    movl $12, %eax
    movl 4+20(%esp), %ebx
    movl 8+20(%esp), %ecx
    int $0x49
    RESTORE_REGS
    ret

preceive:
    SAVE_REGS
    movl $13, %eax
    movl 4+20(%esp), %ebx
    movl 8+20(%esp), %ecx
    int $0x49
    RESTORE_REGS
    ret

preset:
    SAVE_REGS
    movl $14, %eax
    movl 4+20(%esp), %ebx
    int $0x49
    RESTORE_REGS
    ret

wait_clock:
    SAVE_REGS
    movl $15, %eax
    movl 4+20(%esp), %ebx
    int $0x49
    RESTORE_REGS
    ret
