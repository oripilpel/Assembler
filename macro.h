#ifndef ASSEMBLER_MACRO_H
#define ASSEMBLER_MACRO_H

typedef struct {
    struct Macro *head;
    int length;
} MacroList;

typedef struct {
    const char *name;
    struct LineNode *lines_head;
    struct Macro *next;
    int length;
} Macro;

typedef struct LineNode {
    const char *line;
    struct LineNode *next;
} LineNode;

LineNode *create_node(char *line);

Macro *init_macro(char *name);

MacroList *init_macro_list(Macro *head);

void append_line_to_list(Macro *head, char *line);

Macro *find_macro_in_list(MacroList *list, char *name);

void free_macros(MacroList *list);


#endif
