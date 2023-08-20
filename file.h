#ifndef ASSEMBLER_FILE_H
#define ASSEMBLER_FILE_H

#include "macro.h"
#include <stdio.h>

char *get_nth_word(char *line, int n);

void write_macro(FILE *file, Macro *macro);

int is_all_space(char *start);

int macro_expansion(char *filename);

int second_run(char *filename);

#endif
