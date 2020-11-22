nasm -f elf32 enmascarar_asm_externa.asm -o enmascarar_asm.o
sudo gcc -m32  main_c_secuencial.c enmascarar_asm.o -o ejecutable
./ejecutable