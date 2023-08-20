#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "label.h"
#include "macro.h"
#include "utils.h"
#include "operation.h"

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
    char *filename = (char *)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
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
    char *data, *curr_num;
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
                    errors_count++
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
                    L = validate_opcode(line + strlen(label_word), inst); /* validate instruction line and get number of lines or error code if invalid */
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
                L = validate_opcode(line + strlen(label_word), inst); /* validate instruction line and get number of lines or error code if invalid */
                if (L < 0)
                {                                         /* invalid instruction */
                    printf(" in line %d\n", line_number); /* add the line number to STDOUT */
                    errors_count++;
                }
                else
                { /* valid instruction */
                    inst->value = IC;
                    IC += L;
                    append_instruction(inst_table, inst);
                }
            }
        }
        /* initialize for next iteration */

        L = 0;
        current_label = NULL;
        inst = NULL;
        data_valid = INVALID;
        is_label_defining = OFF;
        line_number++;
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
        second_run(input_filename_with_ext, inst_table, table, IC, DC);
    }
    else
    {
        printf("had errors in first run on file.\n");
        return ERROR_CODE;
    }
    return NO_ERROR_CODE;
}

/* fill the unknown addresses from the first run */
int second_run(char *filename, InstructionTable *inst_table, LabelTable *table, int IC, int DC)
{
    Instruction *curr_inst;
    int errors_counter = 0;
    int value;
    Word *curr_word;
    Word *label_word;
    Label *curr_label;
    int IC = 0;
    int L = 0;
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
            curr_label = find_label(label_word->str);
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
            generate_output_files();
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
