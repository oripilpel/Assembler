#ifndef ASSEMBLER_MACRO_H
#define ASSEMBLER_MACRO_H

typedef struct
{
    struct Macro *head;
} MacroList;

typedef struct
{
    const char *name;
    struct LineNode *lines_head;
    struct Macro *next;
} Macro;

typedef struct LineNode
{
    const char *line;
    struct LineNode *next;
} LineNode;

LineNode *create_node(char *line);

void append_macro_to_list(MacroList *list, Macro *macro);

Macro *init_macro(char *name);

MacroList *init_macro_list(Macro *head);

void append_line_to_list(Macro *head, char *line);

Macro *find_macro_in_list(MacroList *list, char *name);

void free_macros(MacroList *list);

#endif
