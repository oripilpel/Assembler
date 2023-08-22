#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH 81
#define MAX_FILE_NAME_LENGTH 50
#define OFF 0
#define ON 1
#define ERROR_CODE -1
#define NO_ERROR_CODE 0
#define MAX_LABEL_LENGTH 31
#define STRING_DATA_TYPE 2
#define DATA_DATA_TYPE 3
#define NONE 0
#define INVALID 0
#define VALID 1
#define REGISTER_TYPE 100
#define IMMEDIATE_TYPE 1
#define LABEL_TYPE 10
#define MAX_NUM_VALUE_OPERATION 511
#define MIN_NUM_VALUE_OPERATION -512
#define MAX_NUM_VALUE 2047
#define MIN_NUM_VALUE -2048
#define SOURCE 1
#define DEST 2
#define MAX_DATA_AND_INSTRUCTIONS 924

/* returns a word structure that has a pointer to the nth word in a line or null if not found and end index of current word */
Word *get_next_word(char *line, int n)
{
    int had_comma = OFF;
    int skipped = 0;
    char *word_start;
    char *curr_word;
    size_t length = 0;
    Word *word;
    line += n;
    word = (Word *)malloc(sizeof(Word));
    if (line == NULL || *line == '\0')
    { /* string is NULL or empty return NULL */
        return NULL;
    }
    while (*line != '\0')
    {
        while (isspace(*line))
        {
            line++;
            skipped++;
        } /* ignore whitespaces */
        if (*line == ',' && !had_comma)
        {
            line++;
            skipped++;
            had_comma = ON;
        }
        if (!isspace(*line))
        {
            length = 0;
            word_start = line;
            while (*line != ',' && !isspace(*line) && *line != '\0')
            {
                line++;
                length++;
            }
            curr_word = (char *)malloc(sizeof(char) * (length + 1)); /* allocate memory for nth word */
            strncpy(curr_word, word_start, length);
            curr_word[length] = '\0';
            word->str = curr_word;
            word->end_idx = n + length + skipped;
            if (!strlen(word))
            {
                return NULL;
            }
            return word;
        }
    }
    return NULL; /* not found next word is empty or comma */
}

/* returns a pointer to the nth word in a line or null if not found */
char *get_nth_word(char *line, int n)
{
    int added_in_curr_iteration = OFF; /* indicator if added 1 to word count in the current iteration */
    char *word_start;
    char *nth_word;
    int word_count = 0;
    size_t length = 0;
    if (line == NULL || *line == '\0')
    { /* string is NULL return NULL */
        return NULL;
    }
    while (*line != '\0')
    {
        if (*line != ',' && !isspace(*line))
        {
            length = 0;
            word_count++;
            added_in_curr_iteration = ON;
            word_start = line;
            while (*line != ',' && !isspace(*line) && *line != '\0')
            {
                line++;
                length++;
            }
            if (n == word_count)
            {                                                           /* found nth word */
                nth_word = (char *)malloc(sizeof(char) * (length + 1)); /* allocate memory for nth word */
                strncpy(nth_word, word_start, length);
                nth_word[length] = '\0';
                return nth_word;
            }
        }
        while (isspace(*line))
            line++; /* ignore whitespaces */
        if (*line == ',')
        {
            line++;
            if (!added_in_curr_iteration)
                word_count++;
        }
        added_in_curr_iteration = OFF;
    }
    return NULL; /* not found nth word return NULL */
}

/* count the words seperated by commas and ignores whitespaces */
char count_words(char *line)
{
    int added_in_curr_iteration = OFF; /* indicator if added 1 to word count in the current iteration */
    int word_count = 0;
    if (line == NULL || *line == '\0')
    { /* string is NULL return 0 */
        return 0;
    }
    while (*line != '\0')
    {
        if (*line != ',' && !isspace(*line))
        {
            word_count++;
            added_in_curr_iteration = ON;
            while (*line != ',' && !isspace(*line) && *line != '\0')
            {
                line++;
            }
        }
        while (isspace(*line))
            line++; /* ignore whitespaces */
        if (*line == ',')
        {
            line++;
            if (!added_in_curr_iteration)
                word_count++;
        }
        added_in_curr_iteration = OFF;
    }
    return word_count;
}

/* returns 1 if all the line is space else 0 */
int is_all_space(char *start)
{
    if (start == NULL)
    {
        return 1;
    }
    while (*start != '\0')
    {
        if (!isspace(*start))
        {
            return 0;
        }
        start++;
    }
    return 1;
}

int validate_number(char *number)
{
    char *number_start;
    char *number_copy;
    int num;

    while (isspace(*number))
    {
        number++;
    }

    number_start = number;

    if (*number == '+' || *number == '-')
        number++; /* it could start with plus or minus sign */

    /* check for NULL or an empty string */
    if (!number || *number == '\0')
        return INVALID;

    while (*number != '\0' && isdigit(*number))
    {
        number++;
    }

    /* allocate memory for the number_copy */
    number_copy = (char *)malloc((number - number_start + 1) * sizeof(char));
    strncpy(number_copy, number_start, number - number_start);
    number_copy[number - number_start] = '\0';

    num = atoi(number_copy);

    if (num > MAX_NUM_VALUE || num < MIN_NUM_VALUE)
    {
        printf("number value %d is not in allowed range\n", num); /* print the integer value */
        free(number_copy);
        return INVALID;
    }

    free(number_copy);

    while (isspace(*number))
    {
        number++;
    }

    /* return 1 to indicate success (all characters were processed) */
    return *number == '\0';
}

/* validate all numbers in string */
int validate_numbers(char *numbers)
{
    char *numbers_copy;
    char *curr_num;
    if (*numbers == ',' || numbers[strlen(numbers) - 1] == ',')
        return INVALID;
    numbers_copy = (char *)malloc(sizeof(char) * (strlen(numbers) + 1));
    strcpy(numbers_copy, numbers);
    numbers_copy[strlen(numbers)] = '\0';
    curr_num = strtok(numbers_copy, ",");
    while (curr_num)
    {
        if (!validate_number(curr_num))
        {
            return INVALID;
        }
        curr_num = strtok(NULL, ",");
    }
    return VALID;
}

/* returns if a given char is within ASCII range */
int is_ascii(char ch)
{
    return (ch >= 0 && ch <= 127);
}

/* return if current word is .data or .string */
int is_data_storing(char *word)
{
    return !strcmp(word, ".data") || !strcmp(word, ".string");
}

/* return if current word is .entry or .extern */
int is_entry_or_extern(char *word)
{
    return !strcmp(word, ".entry") || !strcmp(word, ".extern");
}

/* validating data */
int validate_string_data(char *data)
{
    int valid = ON;
    char *data_end = data + strlen(data) - 1;
    if (!data)
    {
        valid = OFF;
        printf("data is falsy");
    }
    while (isspace(*data_end))
    {
        data_end--;
    }
    if (*data_end != '"' || *data != '"')
    {
        valid = OFF;
        printf("invalid data - data string should start and end with double quotes\n");
    }
    while (*data != '\0')
    {
        if (!is_ascii(*data))
        { /* all chars in string must be ASCII chars */
            valid = OFF;
            printf("invalid data - data string contains non ascii characters\n");
        }
        data++;
    }

    return valid;
}

/* returns data length */
int get_data_length(int data_type, char *data)
{
    if (data_type == STRING_DATA_TYPE)
    {
        return strlen(data) - 1; /* minus two "" and plus one for the \0 in the end */
    }
    return count_words(data);
}

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

/*  append Macro to the end of MacroList  */
void append_macro_to_list_start(MacroList *list, Macro *new_node)
{
    Macro *current = list->head;
    new_node->next = (struct Macro *)current;
    list->head = new_node;
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
    char *data;
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
    free(label->data);
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
    inst->name = name_copy;
    inst->code_flag = OFF;
    inst->entry_flag = OFF;
    inst->external_flag = OFF;
    inst->data_flag = OFF;
    inst->next = NULL;
    inst->instruction = NULL;
    inst->data = NULL;
    inst->value = OFF;
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

typedef struct InstructionTable
{
    struct Instruction *head;
} InstructionTable;

typedef struct Operation
{
    char *opname;
    int num_of_operands;
    int source_allowed;
    int dest_allowed;
    char *opcode;
} Operation;

/* init instruction table */
InstructionTable *init_inst_table(InstructionTable *table)
{
    table = (InstructionTable *)malloc(sizeof(InstructionTable));
    table->head = NULL;
    return table;
}

/* append operation to the end of operation table */
void append_instruction(InstructionTable *table, Instruction *inst)
{
    Instruction *current;
    if (!table->head) /* if table hasen't initialized yet */
    {
        table->head = inst;
        return;
    }
    current = table->head;
    if (!table->head)
    {
        table->head = inst;
        return;
    }
    if (!table->head)
    {
        table->head = inst;
    }
    while (current->next)
    {
        current = current->next;
    }
    current->next = inst;
}

/* returns if the operand allows the  */
int is_op_allowed(int operand_type, int allowed)
{
    if (operand_type > allowed)
    {
        return 0;
    }
    switch (operand_type)
    {
    case LABEL_TYPE: /* 10 */
        if (allowed >= REGISTER_TYPE && allowed - REGISTER_TYPE < 10)
        {
            return 0;
        }
        break;
    case IMMEDIATE_TYPE:     /* 1 */
        return allowed % 10; /* return the reminder */
        break;
    }
    return 1;
}

/* returns the expected operand type */
int get_operand_type(char *operand)
{
    if (*operand == '@') /* register could start with  */
        return REGISTER_TYPE;
    if (isdigit(*operand) || *operand == '+' || *operand == '-') /* number could be a digit or start with plus or minus signs */
        return IMMEDIATE_TYPE;
    if (isalpha(*operand))
        return LABEL_TYPE;
    return ERROR_CODE;
}

/* validating operation line if valid returns number of words needed to code this line */
int validate_opcode(char *line, Instruction *inst)
{
    int num;
    int i = 0;
    int op_type;
    int num_of_operands;
    char *opname;
    char *first_operand, *first_ptr;
    char *second_operand, *second_ptr;
    Word *curr_word;
    Operation *curr_op = NULL;
    Operation operations[16] = {
        {"mov", 2, 111, 110, "0000"},
        {"cmp", 2, 111, 111, "0001"},
        {"add", 2, 111, 110, "0010"},
        {"sub", 2, 111, 110, "0011"},
        {"not", 1, 0, 110, "0100"},
        {"clr", 1, 0, 110, "0101"},
        {"lea", 2, 10, 110, "0110"},
        {"inc", 1, 0, 110, "0111"},
        {"dec", 1, 0, 110, "1000"},
        {"jmp", 1, 0, 110, "1001"},
        {"bne", 1, 0, 110, "1010"},
        {"red", 1, 0, 110, "1011"},
        {"prn", 1, 0, 111, "1100"},
        {"jsr", 1, 0, 110, "1101"},
        {"rts", 0, 0, 0, "1110"},
        {"stop", 0, 0, 0, "1111"}};
    curr_word = get_next_word(line, 0);
    opname = curr_word->str;

    /* validate existance of opname */
    for (i = 0; i < 16; i++)
    {
        if (!strcmp(opname, operations[i].opname))
        {
            curr_op = &operations[i];
        }
    }
    if (!curr_op)
    {
        printf("ERROR - no operation name %s", opname);
        return ERROR_CODE;
    }

    /* validate number of operands */
    num_of_operands = count_words(line + curr_word->end_idx);
    if (num_of_operands != curr_op->num_of_operands)
    {
        printf("ERROR - number of operands %d are not compatible with the operation %s", curr_op->num_of_operands, curr_op->opname);
        return ERROR_CODE;
    }

    /* validate operands sent are allowed */

    curr_word = get_next_word(line, curr_word->end_idx);
    if (curr_word)
    {
        first_operand = curr_word->str;
        first_ptr = first_operand;
    }
    curr_word = get_next_word(line, curr_word->end_idx);
    if (curr_word)
    {
        second_operand = curr_word->str;
        second_ptr = second_operand;
    }

    /* validate the first operand */
    if (num_of_operands)
    {
        op_type = get_operand_type(first_operand);
        switch (op_type)
        {

        case REGISTER_TYPE:
            first_ptr++; /* skip @ */
            /* make sure the r */
            if (*first_ptr != 'r')
            {
                printf("ERROR - register name %s didn't start with r", first_operand);
                return ERROR_CODE;
            }
            first_ptr++;              /* skip r */
            if (first_ptr[1] != '\0') /* number is not between 0-9 */
            {
                printf("ERROR - register number is invalid %s", first_operand);
                return ERROR_CODE;
            }
            num = first_operand[2] - '0'; /* convert the char to numeric */
            if (num >= 8 || num < 0)
            {
                printf("ERROR - register number is invalid %d", num);
                return ERROR_CODE;
            }
            break;
        case IMMEDIATE_TYPE:
            if (!validate_number(first_operand))
            {
                printf("ERROR - number has invalid chars %s", first_operand);
                return ERROR_CODE;
            }
            first_operand = curr_word->str;
            /* validate the number is in valid range and  */
            if (MIN_NUM_VALUE_OPERATION > atoi(first_operand) || MAX_NUM_VALUE_OPERATION < atoi(first_operand))
            {
                printf("ERROR - number %s is not in range of allowed numbers", first_operand);
                return ERROR_CODE;
            }
            break;
        case LABEL_TYPE:
            /* validate only if name is legal */
            if (!validate_label_name(first_operand))
            {
                printf("ERROR - label name %s is inavlid", first_operand);
                return ERROR_CODE;
            }
            break;
        default: /* got ERROR code from get_operand_type */
            printf("ERROR - unidentified operand type %s", first_operand);
            return ERROR_CODE;
            break;
        }
    }

    /* validate the second operand */
    if (num_of_operands == 2)
    {
        op_type = get_operand_type(second_operand);
        switch (op_type)
        {
        case REGISTER_TYPE:
            second_ptr++; /* skip @ */
            /* make sure the r */
            if (*second_ptr != 'r')
            {
                printf("ERROR - register name %s didn't start with r", second_operand);
                return ERROR_CODE;
            }
            second_ptr++;              /* skip r */
            if (second_ptr[1] != '\0') /* number is not between 0-9 */
            {
                printf("ERROR - register number is invalid %s", second_operand);
                return ERROR_CODE;
            }
            num = second_operand[2] - '0'; /* convert the char to numeric */
            if (num >= 8 || num < 0)
            {
                printf("ERROR - register number is invalid %d", num);
                return ERROR_CODE;
            }
            break;
        case IMMEDIATE_TYPE:
            if (!validate_number(second_operand))
            {
                printf("ERROR - number has invalid chars %s", second_operand);
                return ERROR_CODE;
            }
            second_operand = curr_word->str;
            /* validate the number is in valid range and  */
            if (MIN_NUM_VALUE_OPERATION > atoi(second_operand) || MAX_NUM_VALUE_OPERATION < atoi(second_operand))
            {
                printf("ERROR - number %s is not in range of allowed numbers", second_operand);
                return ERROR_CODE;
            }
            break;
        case LABEL_TYPE:
            /* validate only if name is legal */
            if (!validate_label_name(second_operand))
            {
                printf("ERROR - label name %s is inavlid", second_operand);
                return ERROR_CODE;
            }
            break;
        default: /* got ERROR code from get_operand_type */
            printf("ERROR - unidentified operand type %s", second_operand);
            return ERROR_CODE;
        }
    }

    /* initalize instruction */
    inst = (Instruction *)malloc(sizeof(Instruction));

    if (num_of_operands == 1)
    {
        if (is_op_allowed(get_operand_type(first_operand), curr_op->dest_allowed))
        {
            inst->dest_operand = first_operand;
            inst->dest_type = get_operand_type(first_operand);
            inst->source_operand = NULL;
        }
        else
        {
            printf("ERROR - addressing type not allowed for this operation");
            return ERROR_CODE;
        }
    }

    if (num_of_operands == 2)
    {
        if (!is_op_allowed(get_operand_type(first_operand), curr_op->source_allowed) || !is_op_allowed(get_operand_type(second_operand), curr_op->dest_allowed))
        {
            printf("ERROR - addressing type not allowed for this operation");
            return ERROR_CODE;
        }
        else
        {
            inst->source_type = get_operand_type(first_operand);
            inst->dest_type = get_operand_type(second_operand);
            inst->source_operand = first_operand;
            inst->dest_operand = second_operand;
        }
    }

    if (!num_of_operands)
    {
        inst->source_operand = NULL;
        inst->dest_operand = NULL;
        inst->source_type = 0;
        inst->dest_type = 0;
    }

    inst->opname = curr_op->opname;
    inst->opcode = curr_op->opcode;

    /* count and return the number of lines needed based on operands */
    switch (num_of_operands)
    {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        if (get_operand_type(first_operand) == REGISTER_TYPE && get_operand_type(second_operand) == REGISTER_TYPE)
        {
            return 2;
        }
        return 3;
    }
    /* if nothing returned return error code */
    return ERROR_CODE;
}

/*  */
int get_operand_address(Instruction *inst, LabelTable *table, int type)
{
    Label *l;
    if (type == DEST)
    {
        switch (inst->dest_type)
        {
        case REGISTER_TYPE: /* the address is the register's number */
            return atoi(inst->dest_operand + 2);
        case LABEL_TYPE:
            l = find_label(inst->dest_operand, table);
            if (!l)
            {
                printf("ERROR - no label found with name %s\n", inst->dest_operand);
                return ERROR_CODE;
            }
            else
            {
                return l->value;
            }
        }
        /* case IMMEDIATE_TYPE */
        return atoi(inst->dest_operand);
    }
    else
    {
        switch (inst->source_type)
        {
        case REGISTER_TYPE: /* the address is the register's number */
            return atoi(inst->source_operand + 2);

        case LABEL_TYPE:
            l = find_label(inst->source_operand, table);
            if (!l)
            {
                printf("ERROR - no label found with name %s\n", inst->source_operand);
                return ERROR_CODE;
            }
            else
            {
                return l->value;
            }
        }
        /* case IMMEDIATE_TYPE */
        return atoi(inst->source_operand);
    }
}

/* write macro into a file */
void write_macro(FILE *file, Macro *macro)
{
    LineNode *curr_line = macro->lines_head;
    while (curr_line)
    {
        fprintf(file, "%s", curr_line->line);
        curr_line = curr_line->next;
    }
}

/* expanding all macros in filename.as into filename.am */
int macro_expansion(char *filename)
{
    int is_macro = OFF;
    char *first_word;
    Macro *current_macro;
    MacroList *list = NULL;
    FILE *input;
    FILE *output = NULL;
    char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    char *input_filename_with_ext = (char *)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    char *output_filename_with_ext = (char *)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    if (!input_filename_with_ext || !output_filename_with_ext)
    {
        printf("Fatal: failed to allocate required space\n");
        return ERROR_CODE;
    }
    strcpy(input_filename_with_ext, filename);
    strcat(input_filename_with_ext, ".as");
    strcpy(output_filename_with_ext, filename);
    strcat(output_filename_with_ext, ".am");

    input = fopen(input_filename_with_ext, "r");
    if (input == NULL)
    {
        printf("ERROR opening input file: %s\n", input_filename_with_ext);
        return ERROR_CODE;
    }

    while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
    {
        /* the whole line is space or is a comment line */
        if (is_all_space(line) || !strncmp(line, ";", 1))
            continue;
        /* open the file only once and only if there is a line that is not only space */
        if (!output)
        {
            output = fopen(output_filename_with_ext, "w");
            if (output == NULL)
            {
                printf("ERROR opening output file: %s\n", output_filename_with_ext);
                return ERROR_CODE;
            }
        }

        first_word = get_nth_word(line, 1);
        if (is_macro)
        {
            /* end macro */
            if (strncmp(first_word, "endmcro", 7) == 0)
            {
                is_macro = OFF;
                current_macro = NULL;
            }
            else
            {
                /* continue appending line to list */
                append_line_to_list(current_macro, line);
            }
        }
        else
        {
            if (strncmp(first_word, "mcro", 4) == 0)
            {
                /* define new macro */
                is_macro = ON;
                if (!list)
                {
                    current_macro = init_macro(get_nth_word(line, 2));
                    list = init_macro_list(current_macro);
                }
            }
            else
            {
                /* search macro name in list */
                current_macro = find_macro_in_list(list, get_nth_word(line, 1));
                if (current_macro)
                {
                    /* write macro to file */
                    write_macro(output, current_macro);
                }
                else
                {
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

/* fill the unknown addresses from the first run */
int second_run(char *filename, InstructionTable *inst_table, LabelTable *table)
{
    Instruction *curr_inst;
    int errors_counter = 0;
    int value;
    Word *curr_word;
    Word *label_word;
    Label *curr_label;
    FILE *input;
    char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    input = fopen(filename, "r");
    while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
    {
        curr_word = get_next_word(line, 0);
        if (is_label(curr_word->str))
        { /* do nothing */
            curr_word = get_next_word(line, curr_word->end_idx);
        }
        if (is_data_storing(curr_word->str) || !strcmp(".extern", curr_word->str))
        { /* do nothing */
            continue;
        }
        else if (!strcmp(".entry", curr_word->str))
        {
            label_word = get_next_word(line, curr_word->end_idx);
            curr_label = find_label(label_word->str, table);
            if (!curr_label)
            { /* entry label that's not defined in file */
                printf("ERROR - label %s is not defined in file %s\n", label_word->str, filename);
                errors_counter++;
            }
            else
            {
                curr_label->entry_flag = ON;
            }
        }
        curr_label = NULL;
    }
    if (!errors_counter)
    { /* all good no errors in first run and in second run on file */
        /* iterate through instructions and fill out missing addresses */
        curr_inst = inst_table->head;
        while (curr_inst)
        {
            if (curr_inst->source_operand)
            {
                value = get_operand_address(curr_inst, table, SOURCE);
                if (value != ERROR_CODE)
                {
                    curr_inst->source_value = value;
                }
                else
                {
                    errors_counter++;
                }
            }
            if (curr_inst->dest_operand)
            {
                value = get_operand_address(curr_inst, table, DEST);
                if (value != ERROR_CODE)
                {
                    curr_inst->dest_value = value;
                }
                else
                {
                    errors_counter++;
                }
            }
            curr_inst = curr_inst->next;
        }
        /* iterate through labels to fill out missing addresses */
        curr_label = table->head;
        while (curr_label)
        {
            if (curr_label->instruction)
            {
                curr_inst = curr_label->instruction;
                if (curr_inst->source_operand)
                {
                    value = get_operand_address(curr_inst, table, SOURCE);
                    if (value != ERROR_CODE)
                    {
                        curr_inst->source_value = value;
                    }
                    else
                    {
                        errors_counter++;
                    }
                }
                if (curr_inst->dest_operand)
                {
                    value = get_operand_address(curr_inst, table, DEST);
                    if (value != ERROR_CODE)
                    {
                        curr_inst->dest_value = value;
                    }
                    else
                    {
                        errors_counter++;
                    }
                }
            }
            curr_label = curr_label->next;
        }
        if (!errors_counter)
        {
            /*            generate_output_files();*/
        }
        else
        {
            printf("had errors in file not generating output files\n");
            return ERROR_CODE;
        }
    }
    else
    {
        printf("had errors in file not generating output files\n");
        return ERROR_CODE;
    }
    return NO_ERROR_CODE;
}

/* first run through the file all the instructions and labels validate them by schema provided */
int first_run(char *filename)
{
    int data_type, data_valid = INVALID, is_label_defining = OFF;
    int line_number = 1;
    int errors_count = 0;
    Word *curr_word;
    Word *label_word;
    InstructionTable *inst_table;
    Instruction *inst;
    LabelTable *table = NULL;
    Label *current_label = NULL;
    int DC = 0;
    int L = 0;
    int IC = 100;
    FILE *input;
    char *data;
    char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    char *input_filename_with_ext = (char *)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    strcpy(input_filename_with_ext, filename);
    strcat(input_filename_with_ext, ".am");
    if (!input_filename_with_ext)
    {
        printf("Fatal: failed to allocate required space for input_filename_with_ext\n");
        return ERROR_CODE;
    }
    input = fopen(input_filename_with_ext, "r");
    while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
    {
        curr_word = get_next_word(line, 0);
        if (is_label(curr_word->str))
        { /* label defining */
            is_label_defining = ON;
            curr_word = get_next_word(line, curr_word->end_idx);
        }
        if (is_data_storing(curr_word->str))
        { /* storing data */
            if (is_label_defining)
            {
                data_type = !strcmp(curr_word->str, ".data") ? DATA_DATA_TYPE : STRING_DATA_TYPE; /* could be only one of those two */
                data = (char *)malloc(sizeof(char) * (strlen(line) - curr_word->end_idx + 1));
                strcpy(data, line + curr_word->end_idx + 1);
                data_valid = data_type == DATA_DATA_TYPE ? validate_numbers(data) : validate_string_data(data);

                if (!data_valid)
                {
                    errors_count++;
                    printf("ERROR - data invalid in line %d\n", line_number);
                }
                else
                { /* valid data */
                    current_label = init_label(get_next_word(line, 0)->str);
                    if (!current_label)
                    {
                        printf("ERROR - label name invalid in line %d\n", line_number);
                        errors_count++;
                    }
                    else
                    {
                        current_label->data = data;
                        /* valid label name */
                        if (!table) /* haven't initiated label yet */
                        {
                            table = init_table(current_label);
                        }
                        else
                        {
                            append_label_to_table(current_label, table);
                        }
                    }
                }
            }
            else
            { /* trying to save data without label to point it */
                printf("WARNING - trying to save data without pointer to it, discarding data.");
            }
        }
        else if (is_entry_or_extern(curr_word->str))
        {
            if (is_label_defining)
            {
                printf("WARNING : label is irrelevant here.");
            }
            if (!strcmp(".extern", curr_word->str)) /* is .extern type */
            {
                data = (char *)malloc(sizeof(char) * (strlen(line) - curr_word->end_idx + 1));
                strcpy(data, line + curr_word->end_idx);
                if (!table)
                {
                    table = init_table(NULL);
                }
                if (define_extern_labels(data, table))
                {
                    printf(" in line %d", line_number);
                    errors_count++;
                }
            }
        }
        else
        { /* potential instruction */
            if (is_label_defining)
            {
                label_word = get_next_word(line, 0);
                if (find_label(label_word->str, table))
                { /* label name exists */
                    printf("ERROR : found label with name %s", label_word->str);
                    errors_count++;
                }
                else
                {
                    L = validate_opcode(line + strlen(label_word->str), inst); /* validate instruction line and get number of lines or error code if invalid */
                    if (L < 0)
                    {                                         /* invalid instruction */
                        printf(" in line %d\n", line_number); /* add the line number to STDOUT */
                        errors_count++;
                    }
                    else
                    {
                        /* valid instruction */
                        current_label = init_label(label_word->str); /* the label's name is the first word */
                        if (!current_label)                          /* label name's invalid */
                        {
                            printf("ERROR - label name invalid in line %d\n", line_number);
                            errors_count++;
                        }
                        else
                        { /* label's name is valid */

                            current_label->code_flag = ON;
                            current_label->value = IC;
                            current_label->instruction = inst;
                            IC += L;
                            if (!table) /* haven't initiated label yet */
                            {
                                table = init_table(current_label);
                            }
                            else
                            {
                                append_label_to_table(current_label, table);
                            }
                        }
                    }
                }
            }
            else
            {
                /* instruction no label */
                L = validate_opcode(line, inst); /* validate instruction line and get number of lines or error code if invalid */
                if (L < 0)
                {                                         /* invalid instruction */
                    printf(" in line %d\n", line_number); /* add the line number to STDOUT */
                    errors_count++;
                }
                else
                { /* valid instruction */
                    inst->value = IC;
                    IC += L;
                    if (!inst_table)
                    {
                        inst_table = init_inst_table(inst_table);
                    }
                    append_instruction(inst_table, inst);
                }
            }
        }
        if (IC + DC > MAX_DATA_AND_INSTRUCTIONS)
        {
            printf("not enough space for storing all data and instructions\n");
            errors_count++;
        }
        /* initialize for next iteration */

        L = 0;
        current_label = NULL;
        inst = NULL;
        data_valid = INVALID;
        is_label_defining = OFF;
        line_number++;
    }

    /* give data labels addresses after IC */

    current_label = table->head;
    while (current_label)
    {
        if (current_label->data_flag)
        {
            current_label->value = DC + IC;
            DC += get_data_length(data_type, data);
        }
        current_label = current_label->next;
    }
    /* debugging */
    current_label = table->head;
    while (current_label)
    {
        printf("label name:%s with value:%d\n", current_label->name, current_label->value);
        current_label = current_label->next;
    }

    /* debugging */

    if (!errors_count)
    {
        second_run(input_filename_with_ext, inst_table, table);
    }
    else
    {
        printf("had errors in first run on file.\n");
        return ERROR_CODE;
    }
    return NO_ERROR_CODE;
}

int main(int argc, char *argv[])
{

    char *arr[1] = {"test"};
    int i;
    for (i = 0; i < 1; i++)
    {
        macro_expansion(arr[i]);
        first_run(arr[i]);
    }
    return NO_ERROR_CODE;
}