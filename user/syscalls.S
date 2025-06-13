.global exit
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

exit:
    movl $1, %eax
    movl 4(%esp), %ebx
    int $0x49
    ret

cons_write:
    movl $2, %eax
    movl 4(%esp), %ebx
    movl 8(%esp), %ecx
    int $0x49
    ret

start:
    movl $3, %eax
    movl 4(%esp), %ebx
    movl 8(%esp), %ecx
    movl 12(%esp), %edx
    movl 16(%esp), %esi
    movl 20(%esp), %edi
    int $0x49
    ret

waitpid:
    movl $4, %eax
    movl 4(%esp), %ebx
    movl 8(%esp), %ecx
    int $0x49
    ret

kill:
    movl $5, %eax
    movl 4(%esp), %ebx
    int $0x49
    ret

chprio:
    movl $6, %eax
    movl 4(%esp), %ebx
    movl 8(%esp), %ecx
    int $0x49
    ret

getprio:
    movl $7, %eax
    movl 4(%esp), %ebx
    int $0x49
    ret

getpid:
    movl $8, %eax
    int $0x49
    ret

pcreate:
    movl $9, %eax
    movl 4(%esp), %ebx
    int $0x49
    ret

pdelete:
    movl $10, %eax
    movl 4(%esp), %ebx
    int $0x49
    ret

pcount:
    movl $11, %eax
    movl 4(%esp), %ebx
    movl 8(%esp), %ecx
    int $0x49
    ret

psend:
    movl $12, %eax
    movl 4(%esp), %ebx
    movl 8(%esp), %ecx
    int $0x49
    ret

preceive:
    movl $13, %eax
    movl 4(%esp), %ebx
    movl 8(%esp), %ecx
    int $0x49
    ret

preset:
    movl $14, %eax
    movl 4(%esp), %ebx
    int $0x49
    ret

wait_clock:
    movl $15, %eax
    movl 4(%esp), %ebx
    int $0x49
    ret
