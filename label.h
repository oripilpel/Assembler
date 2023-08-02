
#ifndef ASSEMBLER_LABEL_H
#define ASSEMBLER_LABEL_H

typedef struct Label {
    struct Label *next;
    char *name;
    int number;
    int data_flag;
    int code_flag;
    int external_flag;
    int entry_flag;
} Label;

typedef struct LabelTable {
    Label *head;
} LabelTable;

int validate_label_data_by_type(char *name, int data_type, char *data);

Label *init_label(char *name, char *data);

int is_label(char *word);

int get_data_length(char *data);

int is_data_storing(char *word);

LabelTable *init_table(Label *head);

void append_label_to_table(Label *label, LabelTable *table);

int is_entry_or_extern(char *word);

#endif