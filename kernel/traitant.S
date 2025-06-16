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
       
    pushl %eax
    pushl %ecx
    pushl %edx
    
    movw $0x18, %cx
    movw %cx, %ds
    movw %cx, %es
    movw %cx, %fs
    movw %cx, %gs

# appel a la fonction C realisant le traitant
    call tic_PIT
# restauration des registres importants
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
    
    addl $8, %esp
    popf 
    
    pushl %ecx
    movw $0x4b, %cx
    movw %cx, %ds
    movw %cx, %es
    movw %cx, %fs
    movw %cx, %gs
    popl %ecx
    
    iret
