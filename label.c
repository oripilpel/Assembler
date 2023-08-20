#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "operation.h"

typedef struct Label
{
    struct Label *next;
    char *name;
    int value;
    int data_flag;
    int code_flag;
    int external_flag;
    int entry_flag;
    Instruction *instruction;
} Label;

typedef struct LabelTable
{
    Label *head;
} LabelTable;

int is_label(char *word)
{
    return isalpha(*word) && !strcmp(&word[strlen(word) - 1], ":");
}

void free_label(Label *label)
{
    free(label->name);
    free(label);
}

/* check validity of label name and data by type */
int validate_label_name(char *name)
{
    int valid = ON;

    /* validating name */

    int length = 0;
    if (!name)
    {
        valid = OFF;
        printf("invalid label name - label name is null\n");
    }
    while (*name != '\0')
    {
        if (!length && !isalpha(*name))
        {
            valid = OFF;
            printf("invalid label name - label name doesn't start with a letter\n");
            break;
        }
        if (!isalpha(*name) && !isdigit(*name))
        { /* all chars in name must be digit or letters */
            valid = OFF;
            printf("invalid label name - label name contains illegal characters\n");
            break;
        }
        length++;
        name++;
    }
    if (length > MAX_LABEL_LENGTH)
    {
        valid = OFF;
        printf("invalid label name - label name is too long\n");
    }
    return valid;
}

int validate_data(char *line)
{
    /* check each number is continuous and line only contains numbers spaces and commas */
    int num_flag = 0;
    int last_non_space_idx = 0;
    int i = 0;
    char *line_start = line;
    int last_comma = -2;
    while (*line != '\0')
    {
        if (isspace(*line))
        {
            line++;
            continue;
        }
        if ((num_flag && isdigit(*line)) || (!isdigit(*line) && !isspace(*line) && *line != ','))
            return 0;
        while (isdigit(*line))
        {
            num_flag = 1;
            line++;
        }
        if (*line == ',')
        {
            num_flag = 0;
        };
        line++;
    }

    /* second run adjacent comma check*/
    line = line_start;
    while (*line != '\0')
    {
        if (isdigit(*line) || isalpha(*line))
        {
            last_non_space_idx = i;
        }
        else
        {
            if (*line == ',')
            {
                if (last_comma > last_non_space_idx)
                {
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
Label *init_label(char *name)
{
    int valid_name;
    Label *inst = (Label *)malloc(sizeof(Label));
    char *name_copy = (char *)malloc(strlen(name) * sizeof(char));
    strncpy(name_copy, name, strlen(name) - 1); /* copy label's name without : in the end*/
    name_copy[strlen(name) - 1] = '\0';
    if (!*name_copy || !inst)
    {
        printf("Fatal: failed to allocate required space for the label's name\n");
        return NULL;
    }
    valid_name = validate_label_name(name_copy);
    if (!valid_name)
    {
        free(inst);
        free(name_copy);
        return NULL; /* return the label only if it's valid */
    }
    inst->inst->name = name_copy;
    inst->code_flag = OFF;
    inst->entry_flag = OFF;
    inst->external_flag = OFF;
    inst->data_flag = OFF;
    inst->next = NULL;
    inst->instruction = NULL;
    inst->value = NULL;
    return inst;
}

/* init new label table */
LabelTable *init_table(Label *head)
{
    LabelTable *table = malloc(sizeof(LabelTable));
    table->head = head;
    return table;
}

/* find label by name */
Label *find_label(char *name, LabelTable *table)
{
    Label *current;
    if (!table)
        return NULL;
    current = table->head;
    if (!current)
        return NULL;
    while (current)
    {
        if (!strcmp(current->name, name))
            return current;
        current = current->next;
    }
    return NULL;
}

/* insert label into table only if it doesn't already exist */
void append_label_to_table(Label *label, LabelTable *table)
{
    Label *current = table->head;
    if (!table->head)
    {
        table->head = label;
        return;
    }
    if (!table->head)
    {
        table->head = label;
    }
    while (current->next)
    {
        if (!strcmp(current->name, label->name))
        {
            printf("there is another label with the name %s\n", label->name);
            free_label(label); /* this label is no longer needed */
            return;
        }
        current = current->next;
    }
    current->next = label;
}

/* define extern labels */
int define_extern_labels(char *names, LabelTable *table)
{
    Label *temp;
    Label *current_label;
    char *name;
    char *names_copy = (char *)malloc(MAX_LABEL_LENGTH * sizeof(char));
    strcpy(names_copy, names);

    name = strtok(names_copy, ",");
    while (name != NULL)
    {
        current_label = init_label(name);
        if (!current_label)
        {
            printf("label '%s' was invalid\n", name);
            return ERROR_CODE;
        }
        temp = find_label(name, table);
        if (temp->code_flag || temp->entry_flag || temp->data_flag)
        { /* found label with non external flag on */
            printf("label '%s' found with non external flag on\n", name);
            return ERROR_CODE;
        }
        else
        {
            current_label->external_flag = ON;
            current_label->value = 1;
            append_label_to_table(current_label, table);
        }
        name = strtok(NULL, ",");
    }
    return NO_ERROR_CODE;
}
