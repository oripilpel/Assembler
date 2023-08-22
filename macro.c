#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct LineNode
{
    char *line;
    struct LineNode *next;
} LineNode;

typedef struct
{
    const char *name;
    LineNode *lines_head;
    struct Macro *next;
    int length;
} Macro;

typedef struct
{
    Macro *head;
    int length;
} MacroList;

/* create new LineNode  */
LineNode *create_node(char *line)
{
    char *line_copy = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    LineNode *new_node;
    strcpy(line_copy, line);
    new_node = (LineNode *)malloc(sizeof(LineNode));
    if (!*line_copy || !new_node)
    {
        printf("Fatal: failed to allocate required space");
        return NULL;
    }
    if (new_node != NULL)
    {
        new_node->line = line_copy;
        new_node->next = NULL;
    }
    return new_node;
}

/*  Function to initialize an instance of Macro  */
Macro *init_macro(char *name)
{
    Macro *inst = (Macro *)malloc(sizeof(Macro));
    inst->name = name;
    inst->lines_head = NULL;
    inst->length = 0;
    inst->next = NULL;
    return inst;
}

/*  init an instance of MacroList  */
MacroList *init_macro_list(Macro *head)
{
    MacroList *inst = (MacroList *)malloc(sizeof(MacroList));
    inst->head = head;
    inst->length = 1;
    return inst;
}

/* append line to the end of the line list */
void append_line_to_list(Macro *macro, char *line)
{
    LineNode *current = macro->lines_head;
    LineNode *new_node = create_node(line);
    if (!macro->lines_head)
    {
        macro->lines_head = new_node;
        macro->lines_head->next = NULL;
    }
    else
    {
        while (current->next)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

/* find specific macro by name in macro list */
Macro *find_macro_in_list(MacroList *list, const char *name)
{
    Macro *current;
    if (!list)
        return NULL;
    current = list->head;
    while (current)
    {
        if (!strcmp(current->name, name))
        {
            return current;
        }
        current = (Macro *)current->next;
    }
    return NULL;
}

/* free all macros in list */
void free_macros(MacroList *list)
{
    Macro *next_macro;
    LineNode *next_line;
    Macro *current_macro = list->head;
    LineNode *current_line = current_macro->lines_head;
    while (current_macro)
    {
        while (current_line)
        {
            next_line = current_line->next;
            free(current_line);
            current_line = next_line;
        }
        next_macro = (Macro *)current_macro->next;
        free(current_macro);
        current_macro = (Macro *)next_macro;
    }
    free(list);
}
