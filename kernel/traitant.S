# cette directive sert a rendre l'etiquette publique
    .globl traitant_IT_32
    .globl traitant_IT_49
# debut du traitant
traitant_IT_32:
# sauvegarde des registres importants
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    pushl %ebp
# appel a la fonction C realisant le traitant
    call tic_PIT
# restauration des registres importants
    popl %ebp
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
# fin du traitant
    iret

traitant_IT_49:
    subl $24, %esp

    movl %eax, 0(%esp)   # eax
    movl %ebx, 4(%esp)   # ebx
    movl %ecx, 8(%esp)   # ecx
    movl %edx, 12(%esp)  # edx
    movl %esi, 16(%esp)  # esi
    movl %edi, 20(%esp)  # edi

    pushl %esp
    call syscalls_handler_c
    addl $4, %esp

    addl $24, %esp

    iret
