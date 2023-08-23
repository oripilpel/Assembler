assembler: assembler.o file.o macro.o label.o utils.o operation.o
	gcc -g -ansi -Wall -pedantic assembler.o file.o operation.o macro.o label.o utils.o -o assembler
assembler.o: assembler.c file.o constants.h utils.o operation.o
	gcc -g -ansi -Wall -pedantic -c assembler.c -o assembler.o
file.o: file.c label.o macro.o constants.h utils.o
	gcc -g -ansi -Wall -pedantic -c file.c -o file.o
label.o: label.c constants.h utils.o 
	gcc -g -ansi -Wall -pedantic -c label.c -o label.o
macro.o: macro.c constants.h utils.o
	gcc -g -ansi -Wall -pedantic -c macro.c -o macro.o
utils.o: utils.c constants.h
	gcc -g -ansi -Wall -pedantic -c utils.c -o utils.o
operation.o: operation.c label.o constants.h
	gcc -g -ansi -Wall -pedantic -c operation.c -o operation.o