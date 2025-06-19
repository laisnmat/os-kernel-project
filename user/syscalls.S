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
.global clock_settings
.global current_clock
.global cons_echo
.global cons_read
.global ps
.global print_history
.global killall

exit:
    pushl %ebx
    movl $1, %eax
    movl 4+4(%esp), %ebx
    int $0x49
    popl %ebx
    ret

return_exit:
    pushl %ebx
    movl %eax, %ebx
    movl $1, %eax
    int $0x49
    popl %ebx
    ret

cons_write:
    pushl %ebx
    movl $2, %eax
    movl 4+4(%esp), %ebx
    movl 8+4(%esp), %ecx
    int $0x49
    popl %ebx
    ret

start:
    pushl %ebx
    pushl %esi
    pushl %edi
    movl $3, %eax
    movl 4+12(%esp), %ebx
    movl 8+12(%esp), %ecx
    movl 12+12(%esp), %edx
    movl 16+12(%esp), %esi
    movl 20+12(%esp), %edi
    int $0x49
    popl %edi
    popl %esi
    popl %ebx
    ret

waitpid:
    pushl %ebx
    movl $4, %eax
    movl 4+4(%esp), %ebx
    movl 8+4(%esp), %ecx
    int $0x49
    popl %ebx
    ret

kill:
    pushl %ebx
    movl $5, %eax
    movl 4+4(%esp), %ebx
    int $0x49
    popl %ebx
    ret

chprio:
    pushl %ebx
    movl $6, %eax
    movl 4+4(%esp), %ebx
    movl 8+4(%esp), %ecx
    int $0x49
    popl %ebx
    ret

getprio:
    pushl %ebx
    movl $7, %eax
    movl 4+4(%esp), %ebx
    int $0x49
    popl %ebx
    ret

getpid:
    movl $8, %eax
    int $0x49
    ret

pcreate:
    pushl %ebx
    movl $9, %eax
    movl 4+4(%esp), %ebx
    int $0x49
    popl %ebx
    ret

pdelete:
    pushl %ebx
    movl $10, %eax
    movl 4+4(%esp), %ebx
    int $0x49
    popl %ebx
    ret

pcount:
    pushl %ebx
    movl $11, %eax
    movl 4+4(%esp), %ebx
    movl 8+4(%esp), %ecx
    int $0x49
    popl %ebx
    ret

psend:
    pushl %ebx
    movl $12, %eax
    movl 4+4(%esp), %ebx
    movl 8+4(%esp), %ecx
    int $0x49
    popl %ebx
    ret

preceive:
    pushl %ebx
    movl $13, %eax
    movl 4+4(%esp), %ebx
    movl 8+4(%esp), %ecx
    int $0x49
    popl %ebx
    ret

preset:
    pushl %ebx
    movl $14, %eax
    movl 4+4(%esp), %ebx
    int $0x49
    popl %ebx
    ret

wait_clock:
    pushl %ebx
    movl $15, %eax
    movl 4+4(%esp), %ebx
    int $0x49
    popl %ebx
    ret

clock_settings:
    pushl %ebx
    movl $16, %eax
    movl 4+4(%esp), %ebx
    movl 8+4(%esp), %ecx
    int $0x49
    popl %ebx
    ret

current_clock:
    movl $17, %eax
    int $0x49
    ret

cons_echo:
    pushl %ebx
    movl $18, %eax
    movl 4+4(%esp), %ebx
    int $0x49
    popl %ebx
    ret

cons_read:
    pushl %ebx
    movl $19, %eax
    movl 4+4(%esp), %ebx  
    movl 8+4(%esp), %ecx  
    int $0x49
    popl %ebx
    ret
    
ps:
    movl $20, %eax
    int $0x49
    ret

print_history:
    movl $21, %eax
    int $0x49
    ret

killall:
    pushl %ebx
    movl $22, %eax
    movl 4+4(%esp), %ebx
    int $0x49
    popl %ebx
    ret
