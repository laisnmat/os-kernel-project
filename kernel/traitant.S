# cette directive sert a rendre l'etiquette publique
    .globl traitant_IT_32
    .globl traitant_IT_49
# debut du traitant
traitant_IT_32:
# sauvegarde des registres importants

    # Met les segments en mode noyau (0x18 = index 3, ring 0, GDT)
    
    pushf
    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs
    
    movw $0x18, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    
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
    
    popw %gs
    popw %fs
    popw %es
    popw %ds
    popf
    
# fin du traitant
    iret

traitant_IT_49:
    pushf
    pushl %ebp
    pushl %edi   
    pushl %esi   
    pushl %edx   
    pushl %ecx  
    pushl %ebx 
    pushl %eax   

    pushl %esp
    
    pushl %eax 
    movw $0x18, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    popl %eax
    
    call syscalls_handler_c
    
    addl $12, %esp
    
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp
    popf 
    
    pushl %eax
    movw $0x4b, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    popl %eax
    
    iret
