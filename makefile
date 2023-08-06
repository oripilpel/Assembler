assembler: assembler.o file.o macro.o label.o
	gcc -g -ansi -Wall -pedantic assembler.o file.o macro.o label.o -o assembler
assembler.o: assembler.c file.o constants.h
	gcc -g -ansi -Wall -pedantic -c assembler.c -o assembler.o
file.o: file.c label.o macro.o constants.h
	gcc -g -ansi -Wall -pedantic -c file.c -o file.o
label.o: label.c constants.h
	gcc -g -ansi -Wall -pedantic -c label.c -o label.o
macro.o: macro.c constants.h
	gcc -g -ansi -Wall -pedantic -c macro.c -o macro.o