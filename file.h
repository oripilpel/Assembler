#ifndef ASSEMBLER_FILE_H
#define ASSEMBLER_FILE_H

char *get_nth_word(char *line, int n);

void write_macro(FILE *file, Macro *macro);

int is_all_space(char *start);

int macro_expansion(char *filename);

#endif
