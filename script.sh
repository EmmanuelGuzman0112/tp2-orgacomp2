nasm -f elf32 enmascarar_asm_externa.asm -o enmascarar_asm.o
sudo gcc -m32  main.c enmascarar_asm.o -o ejecutable -pthread
./ejecutable a1.bmp a2.bmp amasc.bmp 1153x503
./ejecutable b1.bmp b2.bmp bmasc.bmp 3840x2160
./ejecutable c1.bmp c2.bmp cmasc.bmp 2560x1440
./ejecutable d1.bmp d2.bmp dmasc.bmp 7680x4320
./ejecutable e1.bmp e2.bmp emasc.bmp 3840x2400