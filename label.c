#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

int is_ascii(int ch) {
    return (ch >= 0 && ch <= 127);
}

int is_label(char *word) {
    return isalpha(*word) && !strcmp(&word[strlen(word) - 1], ":");
}

void free_label(Label *label) {
    free(label->name);
    free(label);
}

/* check validity of label name and data by type */
int validate_label_name(char *name) {
    int valid = ON;

    /* validating name */

    int length = 0;
    if (!name) {
        valid = OFF;
        printf("invalid label name - label name is null\n");
    }
    while (*name != '\0') {
        if (!length && !isalpha(*name)) {
            valid = OFF;
            printf("invalid label name - label name doesn't start with a letter\n");
            break;
        }
        if (!isalpha(*name) && !isdigit(*name)) { /* all chars in name must be digit or letters */
            valid = OFF;
            printf("invalid label name - label name contains illegal characters\n");
            break;
        }
        length++;
        name++;
    }
    if (length > MAX_LABEL_LENGTH) {
        valid = OFF;
        printf("invalid label name - label name is too long\n");
    }
    return valid;
}

/* validating data */
int validate_data_by_type(int data_type, char *data) {
    int valid = ON;
    int plus_minus_flag = OFF;
    int number_flag = OFF;
    int last_comma_pos = -2;
    int i = 0;

    if (data) {
        if (data_type == STRING_DATA_TYPE && (data[strlen(data) - 1] != '"' || *data != '"')) {
            valid = OFF;
            printf("invalid label data - label data string should start and end with double quotes\n");
        }
        if (data_type == DATA_DATA_TYPE &&
            ((!isdigit(*data) && strncmp(data, "+", 1) && strncmp(data, "-", 1)) || !isdigit(data[strlen(data) - 1]))) {
            printf("invalid label data - label data data should start with a number or +- sign and end with a number\n");
        }
        while (*data != '\0') {
            if (data_type == STRING_DATA_TYPE) {
                if (!is_ascii(*data)) { /* all chars in name must be digit or letters */
                    valid = OFF;
                    printf("invalid label data - label data string contains non ascii characters\n");
                }
            } else { /* case DATA_DATA_TYPE */

                if (plus_minus_flag &&
                    (!strncmp(data, "+", 1) ||
                     !strncmp(data, "-", 1))) { /* plus minus appeared and another one appears */
                    valid = OFF;
                    printf("invalid label data - label data data contains more than one plus/minus signs per number\n");
                }
                if (number_flag && (!strncmp(data, "+", 1) ||
                                    !strncmp(data, "-", 1))) { /* number appeared and plus or minus appeared after */
                    valid = OFF;
                    printf("invalid label data - label data data number digits and got afterwards plus/minus sign\n");
                }
                if (!strncmp(data, "+", 1) || !strncmp(data, "-", 1)) { /* turn on plus/minus flag */
                    plus_minus_flag = ON;
                }
                if (isdigit(*data)) { /* turn on number flag */
                    number_flag = ON;
                }
                if (*data == ',') {
                    if (last_comma_pos == i - 1) {
                        valid = OFF;
                        printf("invalid label data - label data has two adjacent commas\n");
                    } else {
                        last_comma_pos = i;
                        plus_minus_flag = OFF; /* turn off plus minus flag after comma */
                        number_flag = OFF; /* turn off number flag after comma */
                    }
                }
                if (strncmp(data, "+", 1) && strncmp(data, "-", 1) &&
                    !isdigit(*data) && strncmp(data, ",", 1)) { /* get only numbers or plus minus sign */
                    valid = OFF;
                    printf("invalid label data - label data data illegal character\n");
                }
            }
            data++;
            i++;
        }
    }

    return valid;
}


int validate_data(char *line) {
    /* check each number is continuous and line only contains numbers spaces and commas */
    int num_flag = 0;
    int last_non_space_idx = 0;
    int i = 0;
    char *line_start = line;
    int last_comma = -2;
    while (*line != '\0') {
        if (isspace(*line)) {
            line++;
            continue;
        }
        if ((num_flag && isdigit(*line)) || (!isמםאקdigit(*line) && !isspace(*line) && *line != ','))return 0;
        while (isdigit(*line)) {
            num_flag = 1;
            line++;
        }
        if (*line == ',') {
            num_flag = 0;
        };
        line++;
    }

    /* second run adjacent comma check*/
    line = line_start;
    while (*line != '\0') {
        if (isdigit(*line) || isalpha(*line)) {
            last_non_space_idx = i;
        } else {
            if (*line == ',') {
                if (last_comma > last_non_space_idx) {
                    return 0;
                }
                last_comma = i;
            }
        }
        line++;
        i++;
    }
    return 1;
}


/* init label instance */
Label *init_label(char *name, char *data, int data_type) {
    int valid_name, valid_data;
    Label *inst = (Label *) malloc(sizeof(Label));
    char *name_copy = (char *) malloc(MAX_LABEL_LENGTH * sizeof(char));
    strcpy(name_copy, name);
    if (!*name_copy || !inst) {
        fprintf(stderr, "Fatal: failed to allocate required space for the label's name\n");
        exit(1);
    }
    if (data) { /* validate only if sent */
        valid_name = validate_label_name(name);
        valid_data = validate_data_by_type(data_type, data);
        if (!valid_name || !valid_data) {
            free(inst);
            free(name_copy);
            return NULL; /* return the label only if it's valid */
        }
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

/* find label by name */
Label *find_label(char *name, LabelTable *table) {
    Label *current = table->head;
    if (!current)return NULL;
    while (current) {
        if (!strcmp(current->name, name))return current;
        current = current->next;
    }
    return NULL;
}

/* insert label into table only if it doesn't already exist */
void append_label_to_table(Label *label, LabelTable *table) {
    Label *current = table->head;
    if (!table->head) {
        table->head = label;
        return;
    }
    if (!table->head) {
        table->head = label;
    }
    while (current->next) {
        if (!strcmp(current->name, label->name)) {
            printf("there is another label with the name %s\n", label->name);
            free_label(label); /* this label is no longer needed */
            return;
        }
        current = current->next;
    }
    current->next = label;
}

/* return number of words seperated by comma */
int get_data_length(char *data, int data_type) {
    int length = 0;
    if (data_type == DATA_DATA_TYPE) {
        while (*data != '\0') {
            if (*data == ',')length++;
            data++;
        }
        return length + 1;
    } else {
        return strlen(data);
    }
}

/* define extern labels */
void define_extern_labels(char *names, LabelTable *table, int *DC) {
    Label *temp;
    Label *current_label;
    char *name;
    char *names_copy = (char *) malloc(MAX_LABEL_LENGTH * sizeof(char));
    strcpy(names_copy, names);
    name = strtok(names_copy, ",");
    while (name != NULL) {
        current_label = init_label(name, NULL, 0);
        if (!current_label) {
            printf("label '%s' was invalid\n", name);
            continue;
        }
        temp = find_label(name, table);
        if (!temp) {
            current_label->external_flag = ON;
            current_label->value = *DC++;
            append_label_to_table(current_label, table);
        }
        name = strtok(NULL, ",");
    }
}
