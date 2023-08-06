#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "label.h"
#include "macro.h"

/* return if current word is .data or .string */
int is_data_storing(char *word) {
    return !strcmp(word, ".data") || !strcmp(word, ".string");
}

/* return if current word is .entry or .extern */
int is_entry_or_extern(char *word) {
    return !strcmp(word, ".entry") || !strcmp(word, ".extern");
}

/* returns a pointer to the nth word in a line or null if not found */
char *get_nth_word(char *line, int n) {
    char *word_start;
    int word_count = 0;
    char *nth_word;
    size_t word_length;
    if (line == NULL || *line == '\0') {
        return NULL;
    }

    /* skip leading whitespace characters */
    while (isspace(*line)) {
        line++;
    }

    while (*line != '\0') {
        /* found the start of a word */
        if (!isspace(*line)) {
            word_start = line;
            word_count++;

            /* if this is the nth word return it */
            if (word_count == n) {
                while (*line != '\0' && !isspace(*line)) {
                    line++;
                }

                /* calculate the length of the nth word */
                word_length = line - word_start;

                /* allocate memory for the word and copy it */
                nth_word = (char *) malloc(word_length + 1);
                if (nth_word != NULL) {
                    strncpy(nth_word, word_start, word_length);
                    nth_word[word_length] = '\0';
                    return nth_word;
                } else {
                    return NULL; /* Memory allocation failed */
                }
            }

            /* skip to the end of the current word */
            while (*line != '\0' && !isspace(*line)) {
                line++;
            }
        } else {
            /* skip leading whitespace characters */
            while (isspace(*line)) {
                line++;
            }
        }
    }

    /* if the nth word was not found, return NULL */
    return NULL;
}

/* write macro into a file */
void write_macro(FILE *file, Macro *macro) {
    LineNode *curr_line = macro->lines_head;
    while (curr_line) {
        fprintf(file, "%s", curr_line->line);
        curr_line = curr_line->next;
    }
}

/* returns 1 if all the line is space else 0 */
int is_all_space(char *start) {
    if (start == NULL) {
        return 1;
    }
    while (*start != '\0') {
        if (!isspace(*start)) {
            return 0;
        }
        start++;
    }
    return 1;
}

/* expanding all macros in filename.as into filename.am */
int macro_expansion(char *filename) {
    int is_macro = OFF;
    char *first_word;
    Macro *current_macro;
    MacroList *list = NULL;
    FILE *input;
    FILE *output = NULL;
    char *line = (char *) malloc(MAX_LINE_LENGTH * sizeof(char));
    char *input_filename_with_ext = (char *) malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    char *output_filename_with_ext = (char *) malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    if (!input_filename_with_ext || !output_filename_with_ext) {
        fprintf(stderr, "Fatal: failed to allocate required space");
        exit(1);
    }
    strcpy(input_filename_with_ext, filename);
    strcat(input_filename_with_ext, ".as");
    strcpy(output_filename_with_ext, filename);
    strcat(output_filename_with_ext, ".am");

    input = fopen(input_filename_with_ext, "r");
    if (input == NULL) {
        printf("Error opening input file: %s\n", input_filename_with_ext);
        return ERROR_CODE;
    }

    while (fgets(line, MAX_LINE_LENGTH, input) != NULL) {
        /* the whole line is space or is a comment line */
        if (is_all_space(line) || !strncmp(line, ";", 1))
            continue;
        /* open the file only once and only if there is a line that is not only space */
        if (!output) {
            output = fopen(output_filename_with_ext, "w");
            if (output == NULL) {
                printf("Error opening output file: %s\n", output_filename_with_ext);
                return ERROR_CODE;
            }
        }

        first_word = get_nth_word(line, 1);
        if (is_macro) {
            /* end macro */
            if (strncmp(first_word, "endmcro", 7) == 0) {
                is_macro = OFF;
                current_macro = NULL;
            } else {
                /* continue appending line to list */
                append_line_to_list(current_macro, line);
            }
        } else {
            if (strncmp(first_word, "mcro", 4) == 0) {
                /* define new macro */
                is_macro = ON;
                if (!list) {
                    current_macro = init_macro(get_nth_word(line, 2));
                    list = init_macro_list(current_macro);
                }
            } else {
                /* search macro name in list */
                current_macro = find_macro_in_list(list, get_nth_word(line, 1));
                if (current_macro) {
                    /* write macro to file */
                    write_macro(output, current_macro);
                } else {
                    /* not a macro just a regular line */
                    fprintf(output, "%s", line);
                }
            }
        }
    }
    fclose(input);
    fclose(output);
    free_macros(list);
    free(input_filename_with_ext);
    free(output_filename_with_ext);
    free(first_word);
    free(current_macro);
    return NO_ERROR_CODE;
}

/*  */
int first_run(char *filename) {
    int data_type,data_valid;
    int num_of_extern_names = 0;
    int nth_extern_name = 1;
    int line_number = 1;
    int errors_count = 0;
    LabelTable *table = NULL;
    Label *current_label = NULL;
    int DC = 0;
    int IC = 0;
    FILE *input;
    char *line = (char *) malloc(MAX_LINE_LENGTH * sizeof(char));
    char *input_filename_with_ext = (char *) malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    strcpy(input_filename_with_ext, filename);
    strcat(input_filename_with_ext, ".am");
    if (!input_filename_with_ext) {
        fprintf(stderr, "Fatal: failed to allocate required space for input_filename_with_ext");
        exit(1);
    }
    input = fopen(input_filename_with_ext, "r");
    while (fgets(line, MAX_LINE_LENGTH, input) != NULL) {
        if (is_label(get_nth_word(line, 1))) { /* first word us label */
            if (is_data_storing(get_nth_word(line, 2))) { /* handle data storing label */
                data_type = !strcmp(get_nth_word(line, 2), ".data") ? DATA_DATA_TYPE
                                                                    : STRING_DATA_TYPE; /* only one of two data or string type */
                current_label = init_label(get_nth_word(line, 1), get_nth_word(line, 3), data_type);
                if (current_label) {
                    current_label->value = DC;
                    DC += get_data_length(get_nth_word(line,
                                                       3),data_type); /* update the value of data counter by current label's data length  */
                    current_label->data_flag = ON;
                    if (!table) {
                        table = init_table(current_label);
                    } else {
                        append_label_to_table(current_label, table);
                    }
                }
            }
            if (is_entry_or_extern(get_nth_word(line, 2))) {
                if (is_entry_or_extern(get_nth_word(line, 2))) {
                    if (!strcmp(get_nth_word(line, 2), ".entry")) { /* is entry line */
                        current_label = find_label(get_nth_word(line, 3), table);
                        if (!current_label) {
                            printf("no label found with the name : '%s'", get_nth_word(line, 3));
                        } else if (current_label->external_flag) {
                            printf("ERROR : label '%s' can't be both external and entry", get_nth_word(line, 3));
                        } else {
                            current_label->entry_flag = ON;
                        }
                    } else { /* is extern line */
                        define_extern_labels(get_nth_word(line, 3), table,&DC);
                    }
                }
            }
        } else { /* doesn't start with label name */
            if(is_data_storing(get_nth_word(line,1))){
                data_type = !strcmp(get_nth_word(line, 2), ".data") ? DATA_DATA_TYPE
                                                                    : STRING_DATA_TYPE; /* only one of two data or string type */
                data_valid=validate_data_by_type(get_nth_word(line,2),data_type);
                if(!data_valid){
                    printf("invalid data in line %d\n",line_number);
                    errors_count++;
                }else{
                    DC+=get_data_length(get_nth_word(line, 2),data_type);
                }
            }
           else if (is_entry_or_extern(get_nth_word(line, 1))) {
                if (!strcmp(get_nth_word(line, 1), ".entry")) { /* is entry line */
                    current_label = find_label(get_nth_word(line, 2), table);
                    if (!current_label) {
                        printf("no label found with the name : '%s'", get_nth_word(line, 2));
                    } else if (current_label->external_flag) {
                        printf("ERROR : label '%s' can't be both external and entry", get_nth_word(line, 2));
                    } else {
                        current_label->entry_flag = ON;
                    }
                } else { /* is extern line */
                    define_extern_labels(get_nth_word(line, 2), table);
                }
            }
        }
        line_number++;
    }
    return NO_ERROR_CODE;
}
