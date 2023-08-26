#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H

typedef struct Word
{
    char *str;
    int end_idx;
} Word;

Word *get_next_word(char *line, int n);

char *get_nth_word(char *line, int n);

int count_words(char *line);

int is_all_space(char *start);

int validate_numbers(char *numbers);

int validate_number(char *number);

int is_ascii(char ch);

int is_data_storing(char *word);

int is_entry_or_extern(char *word);

int validate_string_data(char *data);

int get_data_length(int data_type, char *data);

char *int_to_binary_string(int number, int num_bits);

void init_string(char *s);

char *ascii_char_to_binary(char ascii_char);

void free_word(Word *word);

#endif
