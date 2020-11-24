global enmascarar_asm_externa

section .data


section .text 
 
enmascarar_asm_externa:
    push ebp ; enter
    mov ebp, esp  ; enter
    
    ;variables que vienen de C:
    ;
    ;
    ;bufferImgA=EBP+8
    ;bufferImgB=EBP+12
    ;bufferMasc=EBP+16
    ;cant=EBP+20

    
    mov ebx,[EBP+20] ;cant bytes por img
    xor edx,edx  ;uso edx como acumulador para ciclar
    
ciclo:

    MOVQ mm0,[(ebp+8)+edx] ;imagen a
   
    MOVQ mm1,[(ebp+12)+edx];imagen b
    
    MOVQ mm2,[(ebp+16)+edx];mascara
    
    
    PAND mm1, mm2 ;mantiene la imagen b cuando el pixel es negro 
    PANDN mm2, mm0 ;mantiene la imagen a cuando el pixel es distinto de FFFFFF
    POR mm1, mm2
    
    mov eax,[ebp+8]
    MOVQ [eax+ecx], mm1
    
    add edx,8

    cmp ebx,edx
    JNE ciclo
     
    
fin:
    
    mov ebp,esp ;Reset Stack (leave)
    pop ebp ;Restore (leave)
    
    ret