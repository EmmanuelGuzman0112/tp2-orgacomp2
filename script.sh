nasm -f elf32 enmascarar_asm_externa.asm -o enmascarar_asm.o
sudo gcc -m32  main.c enmascarar_asm.o -o main -pthread
./main a1.bmp a2.bmp amasc.bmp 1153x503
./main b1.bmp b2.bmp bmasc.bmp 3840x2160
./main c1.bmp c2.bmp cmasc.bmp 2560x1440
./main d1.bmp d2.bmp dmasc.bmp 7680x4320
./main e1.bmp e2.bmp emasc.bmp 3840x2400