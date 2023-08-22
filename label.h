#ifndef ASSEMBLER_LABEL_H
#define ASSEMBLER_LABEL_H

typedef struct Instruction
{
    struct Instruction *next;
    int value;
    char *opname;
    char *source_operand;
    char *dest_operand;
    char *opcode;
    int source_value;
    int dest_value;
    int source_type;
    int dest_type;
} Instruction;

typedef struct Label
{
    struct Label *next;
    char *data;               /* label data if exist */
    char *name;               /* label name */
    int value;                /* label address in memory */
    int data_flag;            /* if it contains data it's 1 else 0 */
    int code_flag;            /* if it contains instruction it's 1 else 0 */
    int external_flag;        /* if it external label it's 1 else 0 */
    int entry_flag;           /* if it external label it's 1 else 0 */
    Instruction *instruction; /* if it has code flag it's the instruction  */
} Label;

typedef struct LabelTable
{
    Label *head; /* the first instruction in linked list */
} LabelTable;

int validate_label_name(char *name);

Label *init_label(char *name);

int is_label(char *word);

Label *find_label(char *name, LabelTable *table);

LabelTable *init_table(Label *head);

int define_extern_labels(char *names, LabelTable *table);

void append_label_to_table(Label *label, LabelTable *table);

#endif