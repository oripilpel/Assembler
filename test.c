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

typedef struct Word
{
    char *str;
    int end_idx;
} Word;

/* returns a word structure that has a pointer to the nth word in a line or null if not found and end index of current word */
Word *get_next_word(char *line, int n)
{
    int had_comma = OFF;
    int skipped = 0;
    line += n;
    char *word_start;
    char *curr_word;
    size_t length = 0;
    Word *word = (Word *)malloc(sizeof(Word));
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

/* validate each number in .data data */
int validate_number(char *number)
{
    while (isspace(*number))
    {
        number++;
    }
    if (*number == '+' || *number == '-')
        number++; /* it could start with plus or minus sign */
    if (!number || *number == '\0' || !strlen(number))
        return INVALID;
    while (*number != '\0' && isdigit(*number))
    {
        number++;
    }
    while (isspace(*number))
    {
        number++;
    }
    return *number == '\0';
}

/* validate all numbers in string */
int validate_numbers(char *numbers)
{
    if (*numbers == ',' || numbers[strlen(numbers) - 1] == ',')
        return INVALID;
    char *numbers_copy = (char *)malloc(sizeof(char) * (strlen(numbers) + 1));
    strcpy(numbers_copy, numbers);
    numbers_copy[strlen(numbers)] = '\0';
    char *curr_num = strtok(numbers_copy, ",");
    int data_valid = VALID;
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
    if (data)
    {
        if (data[strlen(data) - 1] != '"' || *data != '"')
        {
            valid = OFF;
            printf("ERROR - invalid data - data string should start and end with double quotes\n");
        }
        while (*data != '\0')
        {
            if (!is_ascii(*data))
            { /* all chars in string must be ASCII chars */
                valid = OFF;
                printf("ERROR - invalid data - data string contains non ascii characters\n");
            }
            data++;
        }
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

typedef struct Label
{
    struct Label *next;
    char *name;
    int value;
    int data_flag;
    int code_flag;
    int external_flag;
    int entry_flag;
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
        printf("ERROR - invalid label name - label name is null\n");
    }
    while (*name != '\0')
    {
        if (!length && !isalpha(*name))
        {
            valid = OFF;
            printf("ERROR - invalid label name - label name doesn't start with a letter\n");
            break;
        }
        if (!isalpha(*name) && !isdigit(*name))
        { /* all chars in name must be digit or letters */
            valid = OFF;
            printf("ERROR - invalid label name - label name contains illegal characters\n");
            break;
        }
        length++;
        name++;
    }
    if (length > MAX_LABEL_LENGTH)
    {
        valid = OFF;
        printf("ERROR - invalid label name - label name is too long\n");
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
    char *name_copy = (char *)malloc(MAX_LABEL_LENGTH * sizeof(char));
    strcpy(name_copy, name);
    if (!*name_copy || !inst)
    {
        printf("Fatal: failed to allocate required space for the label's name\n");
        exit(1);
    }
    valid_name = validate_label_name(name);
    if (!valid_name)
    {
        free(inst);
        free(name_copy);
        return NULL; /* return the label only if it's valid */
    }
    inst->name = name_copy;
    inst->next = NULL;
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
            printf("ERROR - there is another label with the name %s\n", label->name);
            free_label(label); /* this label is no longer needed */
            return;
        }
        current = current->next;
    }
    current->next = label;
}

/* define extern labels */
void define_extern_labels(char *names, LabelTable *table, int *DC, int *errors_count)
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
            printf("ERROR - label '%s' was invalid\n", name);
            errors_count++;
            continue;
        }
        temp = find_label(name, table);
        if (!temp)
        {
            current_label->external_flag = ON;
            current_label->value = *DC++;
            append_label_to_table(current_label, table);
        }
        name = strtok(NULL, ",");
    }
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
        printf("Fatal: failed to allocate required space\n");
        exit(1);
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
        exit(1);
    }
    strcpy(input_filename_with_ext, filename);
    strcat(input_filename_with_ext, ".as");
    strcpy(output_filename_with_ext, filename);
    strcat(output_filename_with_ext, ".am");

    input = fopen(input_filename_with_ext, "r");
    if (input == NULL)
    {
        printf("ERROR - ERROR opening input file: %s\n", input_filename_with_ext);
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
                printf("ERROR - ERROR opening output file: %s\n", output_filename_with_ext);
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

int first_run(char *filename)
{
    int data_type, data_valid = INVALID, is_label_defining = OFF;
    int line_number = 1;
    int errors_count = 0;
    Word *curr_word;
    Word *label_word;

    LabelTable *table = NULL;
    Label *current_label = NULL;
    int DC = 0;
    int IC = 0;
    FILE *input;
    char *data, *curr_num;
    char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    char *input_filename_with_ext = (char *)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    strcpy(input_filename_with_ext, filename);
    strcat(input_filename_with_ext, ".am");
    if (!input_filename_with_ext)
    {
        printf("Fatal: failed to allocate required space for input_filename_with_ext\n");
        exit(1);
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
                data_type = !strcmp(get_next_word(line, curr_word->end_idx)->str, ".data") ? DATA_DATA_TYPE
                                                                                           : STRING_DATA_TYPE; /* could be only one of those two */
                if (data == DATA_DATA_TYPE)
                {
                    data = (char *)malloc(sizeof(char) * (strlen(line) - curr_word->end_idx + 1));
                    strcpy(data, line + curr_word->end_idx);
                    data_valid = validate_numbers(data);
                }
                else /* is .string data type  */
                {
                    data = (char *)malloc(sizeof(char) * (strlen(line) - curr_word->end_idx + 1));
                    strcpy(data, line + curr_word->end_idx);
                    data_valid = validate_string_data(data);
                }
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
                        /* valid label name */
                        if (!table) /* haven't initiated label yet */
                        {
                            table = init_table(current_label);
                        }
                        else
                        {
                            append_label_to_table(current_label, table);
                        }
                        current_label->value = DC;
                        DC += get_data_length(data_type, data);
                    }
                }
            }
            else
            { /* trying to save data without label to point it */
                printf("Warning - trying to save data without pointer to it, discarding data.\n");
            }
        }
        else if (is_entry_or_extern(curr_word->str))
        {
            if (is_label_defining)
            {
                printf("WARNING - label is irrelevant here.\n");
            }
            if (!strcmp(".extern", curr_word->str)) /* is .extern type */
            {
                data = (char *)malloc(sizeof(char) * (strlen(line) - curr_word->end_idx + 1));
                strcpy(data, line + curr_word->end_idx);
                if (!table)
                {
                    table = init_table(NULL);
                }
                define_extern_labels(data, table, &DC, &errors_count);
            }
        }
        else
        { /* instruction */
            if (is_label_defining)
            {
                label_word = get_next_word(line, 0);
                if (find_label(label_word->str, table))
                { /* label name exists */
                    printf("ERROR - found label with name %s\n", label_word->str);
                    errors_count++;
                }
                else
                {
                    current_label = init_label(label_word->str); /* the label's name is the first word */
                    if (!current_label)                          /* label name's invalid */
                    {
                        printf("ERROR - label name invalid in line %d\n", line_number);
                        errors_count++;
                    }
                    else
                    { /* label's name is valid */
                        current_label->code_flag = ON;
                    }
                }
            }
        }
        data_valid = INVALID;
        is_label_defining = OFF; /* initialize it for next iteration */
        line_number++;
    }

    /* debugging */
    current_label = table->head;
    while (current_label)
    {
        printf("label name:%s\n", current_label->name);
        current_label = current_label->next;
    }

    /* debugging */

    return errors_count ? ERROR_CODE : NO_ERROR_CODE;
}

int main(int argc, char *argv[])
{
    macro_expansion("file");
    first_run("file");
    return NO_ERROR_CODE;
}