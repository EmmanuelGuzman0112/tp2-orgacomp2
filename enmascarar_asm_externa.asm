global enmascarar_asm_externa

section .data

tiraBlanco db 255,255,255,255,255,255,255,255 ;uso para aplicar instrucciones simd
colorBlanco dd 255                            ;la uso igual que la tira pero para instrucciones de uno en uno
colorNegro dd 0                               ;color negro para negar la mascara (sin usar simd)


acumulador dd 0                               ;acumulador para cuando aplico mascara sin instrucciones SIMD                          
desplazamiento dd 0                           ;desplazamiento para cuando aplico mascara sin instrucciones SIMD  
aux dd 0                                      ;auxiliar para guardar datos ya que los 4 registros de prop. general no alcanzan              

section .text 
 
enmascarar_asm_externa:
    push ebp ; enter
    mov ebp, esp  ; enter
    
    ;variables que vienen de C:
    ;
    ;buffer Img1=EBP+8
    ;buffer Img2=EBP+12
    ;buffer Mascara=EBP+16
    ;cant de bits=EBP+20

   

    mov ebx,0       ;limpio ebx para usarlo para moverme en la direccion a la que apunta cada imagen
    mov edx,[EBP+20];cant bits por img
    movq mm3,qword[tiraBlanco] ;cargo tira para usar operaciones simd



ciclo:

    ;saltar si es menor a 8 | se da en el caso que las img tengan menos de 8 bits o cuando estoy en los ultimos bits de las img
    cmp edx,8
    jl menorA8
    

    mov eax,[ebp+16]   ;mascara
    MOVQ mm0,[eax+ebx] ;guardo contenido de mascara en mm0 | me desplazo con ebx
    mov eax,[ebp+8]    ;img1
    MOVQ mm1,[eax+ebx] ;guardo contenido de img1 en mm0 | me desplazo con ebx
    mov eax,[ebp+12]   ;img2
    MOVQ mm2,[eax+ebx] ;guardo contenido de img2 en mm0 | me desplazo con ebx
   
    pand mm2,mm0  ;borro de la img 2 los canales de pixeles donde la mascara es negra 
    pxor mm0,mm3  ;creo la negacion de la mascara | mm3 = (0,0,0,0,0,0,0,0) -> negro en los 8
    pand mm1, mm0 ;borro de la img 1 los canales de pixeles donde la imagen es blanca
    por mm1, mm2  ;sumamos las dos imagenes alteradas y guardo el resultado en la imagen 1
    
    mov eax,[ebp+8] ; vuelvo a cargar img1 en eax para guardar cambios
    MOVQ qword[eax+ebx], mm1 ;guardo lo obtenido en mm1 (resultados parciales) en la img1 (con su desplazamiento en ebx) 
    
    SUB edx,8 ;bajo en 8 (porque cada posicion en registros mmx tiene esa cantidad de bytes) 
    add ebx,8 ;aumento en 8 el desplazamiento en memoria | misma situacion que caso anterior
    JMP ciclo

menorA8:
    ;valido que edx no sea 0 - pasaria solo cuando la cant de bytes sea un multiplo de 8
    cmp edx,0    
    je fin

    ;A partir de acá la lógica es la misma pero en vez de aplicar la mascara en 8 bits,se hace de uno en uno 
    ;(se hará a lo sumo 7 veces y solo en casos que la cant de bits no sea múltiplo de 8)
    
    ;guardo el acumulador que desciende
    mov dword[acumulador],edx
    ;guardo el desplazamiento | notar que viene del lugar donde quedo en el ciclo anterior
    mov dword[desplazamiento],ebx
    ;guardo para aplicar ese color en mascara
    mov edx, [colorBlanco]

 
menorA8Ciclo:
    ;las operaciones se ejecutaran de forma tal que se puedan usar todos los registros sin que se pisen entre sí

    mov eax, [desplazamiento] ;guardo en eax el desplazamiento de cada img

    mov ebx,[ebp+12] ;img2
    mov ecx,[ebx+eax] ;guardo contenido de img2 en ecx | me desplazo con eax

    mov ebx,[ebp+16];mascara
    mov edx,[ebx+eax];guardo contenido de mascara en edx | me desplazo con eax

    and ecx,edx ; borro de la img 2 los canales de pixeles donde la mascara es negra 
    xor edx,[colorNegro]; creo la negacion de la mascara | aca tengo el color negro que en mmx tenia en mm3

    mov dword[aux],edx ;guardo en variable aux el bit que obtuve de la operacion anterior para poder usar edx
    
    mov edx,[desplazamiento] ;guardo en edx el desplazamiento de cada img

    mov ebx,[ebp+8] ;img1
    mov eax,[ebx+edx];guardo contenido de mascara en eax | me desplazo con edx
    
    
    
    and eax,[aux] ;borro de la img 1 los canales de pixeles donde la imagen es blanca
    or eax,ecx ;sumamos las dos imagenes alteradas y guardo el resultado en la imagen 1


    mov ecx,[ebp+8] ; vuelvo a cargar img1 en ecx para guardar cambios
    mov [ecx+edx], eax ;guardo lo obtenido en eax (resultados parciales) en la img1 (con su desplazamiento en edx) 

    SUB dword[acumulador],1 ;bajo el acumulador que controla el ciclo
    add dword[desplazamiento],1      ;subo el desplazamiento

    ;repito el ciclo mientras el acumulador no sea 0
    cmp dword[acumulador],0
    JNE menorA8Ciclo
    
fin:
   
    mov ebp,esp ;Reset Stack (leave)
    pop ebp ;Restore (leave)
    
    ret