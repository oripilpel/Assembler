assembler: assembler.o
	gcc -: file.o macro.o
assembler.o: file.o macro.o
	gcc -g -ansi -Wall -pedantic file.o macro.o -o assembler.o
file.o: file.c constants.h
	gcc -g -ansi -Wall -pedantic -c file.c -o file.o
macro.o: macro.c constants.h
	gcc -g -ansi -Wall -pedantic -c macro.c -o macro.o