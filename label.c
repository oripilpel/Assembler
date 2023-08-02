#include "constants.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"

typedef struct Label {
    struct Label *next;
    char *name;
    int value;
    int data_flag;
    int code_flag;
    int external_flag;
    int entry_flag;
} Label;

typedef struct LabelTable {
    Label *head;
} LabelTable;

/* check validity of label name and data by type */
int validate_label_data_by_type(char *name, int data_type, char *data) {
    int valid = ON;
    int plus_minus_flag = OFF;
    int number_flag = OFF;
    char *curr_number = (char *) malloc(MAX_LABEL_LENGTH * sizeof(char));

    /* validating name */

    int length = 0;
    if (!*name) {
        valid = OFF;
        printf("invalid label name - label name is null");
    }
    while (*word != '\n') {
        if (!isalpha(*name) || !isdigit(*name)) { /* all chars in name must be digit or letters */
            valid = OFF;
            printf("invalid label name - label name contains illegal characters");
        }
        length++;
        word++;
    }
    if (length > MAX_LABEL_LENGTH) {
        valid = OFF;
        printf("invalid label name - label name is too long");
    }

    /* validating data */

    if (data) {
        if (data_type == STRING_DATA_TYPE && (*word[strlen(word) - 1] != '"' || *word[0] != '"')) {
            valid = OFF;
            printf("invalid label data - label data string should start and end with double quotes");

        }
        if (data_type == DATA_DATA_TYPE && (*word[strlen(word) - 1] == "," || *word[0] == ",")) {
            printf("invalid label data - label data data should not have comma in start of end");
        }
        while (*data != '\n') {
            if (data_type == STRING_DATA_TYPE) {
                if (!isascii(*data)) { /* all chars in name must be digit or letters */
                    valid = OFF;
                    printf("invalid label data - label data string contains non ascii characters");
                }
            } else {/* case DATA_DATA_TYPE */
                if (plus_minus_flag &&
                    (!strncmp(data, "+", 1) ||
                     !strncmp(data, "-", 1))) {/* plus minus appeared and another one appears */
                    valid = OFF;
                    printf("invalid label data - label data data contains more than one plus/minus signs");
                }
                if (number_flag && (!strncmp(data, "+", 1) ||
                                    !strncmp(data, "-", 1))) { /* number appeared and plus or minus appeared after */
                    valid = OFF;
                    printf("invalid label data - label data data illegal number digits and afterwards plus/minus sign");
                }
                if (!strncmp(data, "+", 1) || !strncmp(data, "-", 1)) { /* turn on plus/minus flag */
                    plus_minus_flag = ON;
                }
                if (isdigit(*data)) { /* turn on number flag */
                    number_flag = ON;
                }
                if (strncmp(data, "+", 1) && strncmp(data, "-", 1) &&
                    !isdigit(*data)) {/* get only numbers or plus minus sign */
                    valid = OFF;
                    printf("invalid label data - label data data illegal character");
                }

            }
            data++;
        }
    }

    return valid;
}

/* init label instance */
Label *init_label(char *name, char *data) {
    int data_type;
    int valid;
    Label *inst = (LineNode *) malloc(sizeof(Label));
    char *name_copy = (char *) malloc(MAX_LABEL_LENGTH * sizeof(char));
    strcpy(name_copy, name);
    if (!*name_copy || !inst) {
        fprintf(stderr, "Fatal: failed to allocate required space for the label's name");
        exit(1);
    }
    if (data) { /* validate only if sent */
        data_type = !strcmp(word, ".data") ? DATA_DATA_TYPE
                                           : STRING_DATA_TYPE; /* only one of two data or string type */
        valid = validate_label_data_by_type(name, data_type, data);
        if (!valid)return NULL; /* return the label only if it's valid */
    }
    inst->name = name_copy;
    inst->next = NULL;
    return inst;
}

/* init new label table */
LabelTable *init_table(Label *head) {
    LabelTable *table = malloc(sizeof(LabelTable));
    table->head = head;
    return table;
}

/* insert label into table only if it doesn't already exist */
void append_label_to_table(Label *label, LabelTable *table) {
    Label *current = table->head;
    if (!table->head) {
        table->head = label;
    }
    do {
        if (!strcmp(current->name, label->name)) {
            printf("there is another label with the name %s", label->name);
            return;
        }
        current = current->next;
    } while (current->next);
    current->next = label;
}

/* return number of words seperated by comma */
int get_data_length(char *line) {
    int length = 0;
    int last_comma_location = 0;
    while (*line != "/n") {
        if (*line != ",") {
            line++;
        } else {
            if (last_comma_location == line - 1) { /* two commas with no value between them  */
                printf("error in data there are two commas beside each other");
                return 0;
            } else {
                length++;
                last_comma_location = line;
            }
        }
    }
    return length;
}

/* returns if a word is a label definition */
int is_label(char *word) {
    return isalpha(*word) && *word[strlen(word) - 1] == ':';
}

/* return if current label define is of data storing type */
int is_data_storing(char *word) {
    return !strcmp(word, ".data") || !strcmp(word, ".string");
}

int is_entry_or_extern(char *word) {
    return !strcmp(word, ".entry") || !strcmp(word, ".extern");
}
