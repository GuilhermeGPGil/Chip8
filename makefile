emulator: main.o emulator.o chip8.o display.o
	gcc main.o emulator.o chip8.o display.o -o emulator

display.o: display.c
	gcc -c display.c -o display.o

chip8.o: chip8.c
	gcc -c chip8.c -o chip8.o

main.o: main.c
	gcc -c main.c -o main.o

emulator.o: emulator.c
	gcc -c emulator.c -o emulator.o

clean:
	rm emulator
	rm *.o

run:
	./emulator
