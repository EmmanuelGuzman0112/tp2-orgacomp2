global enmascarar_asm_externa

section .data

aux db '0' ;para el color blanco

section .text 
 
enmascarar_asm_externa:
    push ebp ; enter
    mov ebp, esp  ; enter
    
    ;variables que vienen de C:
    ;
    ;
    ;bufferImgA=EBP+8
    ;bufferImgB=EBP+16
    ;bufferMasc=EBP+24
    ;cant=EBP+32

    ; for (int i=0; i<cant; i++){  //tantas iteraciones como bytes tiene el archivo
    ;    if (*(mask+i) != aux)
    ;         *(a+i) = *(b+i);

    ; }
    
    
    xor edx,edx  ;uso edx como acumulador para ciclar

    movq mm0,qword[EBP+8+8*edx] ; bufferImgA
    movq mm1,qword[EBP+8*edx]; bufferImgB
    movq mm2,qword[EBP+8*edx]; bufferMasc
    movq mm3,qword[ebp+32]; cantidad bytes de imagenes -> quizas podria ir en un registro de prop. general
    ;pcmpeqb mm0,mm1 compara mm0 con mm1. Si son iguales deja ff sino 00. Quizas seria util para la mascara

    mov ebp,esp ;Reset Stack (leave)
    pop ebp ;Restore (leave)
    
    ret
