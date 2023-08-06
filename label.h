
#ifndef ASSEMBLER_LABEL_H
#define ASSEMBLER_LABEL_H

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

int validate_data_by_type(int data_type, char *data) ;

Label *init_label(char *name, char *data, int data_type);

int is_label(char *word);

Label *find_label(char *name, LabelTable *table);

int get_data_length(char *data,int data_type);

LabelTable *init_table(Label *head);

void define_extern_labels(char *names, LabelTable *table);

void append_label_to_table(Label *label, LabelTable *table);

#endif