#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "label.h"
#include "macro.h"
#include "utils.h"
#include "operation.h"
/* returns converted binary string  */
char *binary_to_hex(const char *binary)
{
    char *hex;
    int i = 0;
    int value = 0;
    hex = (char *)malloc(3 * sizeof(char));
    for (i = 0; i < 12; i++)
    {
        value <<= 1;
        value += binary[i] - '0';
    }
    sprintf(hex, "%02X", value);
    return hex;
}

/* write line into file with or without counter */
void write_line(char *line, int counter, FILE *output)
{
    if (counter)
    {
        fprintf(output, "%s\t\t%d\n", line, counter);
    }
    else
    {
        fprintf(output, "%s\n", line);
    }
}

/* write instruction data to file  */
void handle_instruction(Instruction *inst, int *counter, FILE *object_output, FILE *ob_output)
{
    int i;
    /* write first line */
    char *binary_value, *hex_value;
    char *curr_line;
    curr_line = (char *)malloc(OUTPUT_LINE_LENGTH * sizeof(char));
    init_string(curr_line);
    switch (inst->source_type)
    {
    case IMMEDIATE_TYPE:
        curr_line[2] = '1';
        break;
    case LABEL_TYPE:
        curr_line[2] = '1';
        curr_line[1] = '1';
        break;
    case REGISTER_TYPE:
        curr_line[2] = '1';
        curr_line[0] = '1';
        break;
    }
    curr_line[3] = inst->opcode[0];
    curr_line[4] = inst->opcode[1];
    curr_line[5] = inst->opcode[2];
    curr_line[6] = inst->opcode[3];
    switch (inst->dest_type)
    {
    case IMMEDIATE_TYPE:
        curr_line[9] = '1';
        break;
    case LABEL_TYPE:
        curr_line[9] = '1';
        curr_line[8] = '1';
        break;
    case REGISTER_TYPE:
        curr_line[9] = '1';
        curr_line[7] = '1';
        break;
    }
    write_line(curr_line, (*counter)++, object_output);
    hex_value = binary_to_hex(curr_line);
    write_line(hex_value, 0, ob_output);
    init_string(curr_line);

    /* if only one operand */
    if (inst->dest_operand && !inst->source_operand)
    {
        i = 0;
        if (inst->dest_type == REGISTER_TYPE)
        {
            binary_value = int_to_binary_string(inst->source_value, 5);
            while (i < 5)
            {
                curr_line[i + 5] = binary_value[i];
                i++;
            }
        }
        else
        {
            if (inst->dest_type == LABEL_TYPE)
            {
                if (inst->dest_value == 1)
                {
                    curr_line[11] = '1';
                }
                else
                {
                    curr_line[10] = '1';
                    binary_value = int_to_binary_string(inst->dest_value, 10);
                    while (i < 10)
                    {
                        curr_line[i] = binary_value[i];
                        i++;
                    }
                }
            }
            else
            {

                binary_value = int_to_binary_string(inst->dest_value, 10);
                while (i < 10)
                {
                    curr_line[i] = binary_value[i];
                    i++;
                }
            }
        }
        write_line(curr_line, (*counter)++, object_output);
        hex_value = binary_to_hex(curr_line);
        write_line(hex_value, 0, ob_output);
        init_string(curr_line);
    }

    /* code two operands */
    else if (inst->dest_operand && inst->source_operand)
    {
        i = 0;
        if (inst->dest_type == REGISTER_TYPE && inst->source_type == REGISTER_TYPE)
        {
            binary_value = int_to_binary_string(inst->source_value, 5);
            while (i < 5)
            {
                curr_line[i] = binary_value[i];
                i++;
            }
            binary_value = int_to_binary_string(inst->dest_value, 5);
            i = 0;
            while (i < 5)
            {
                curr_line[i + 5] = binary_value[i];
                i++;
            }
            write_line(curr_line, (*counter)++, object_output);
            hex_value = binary_to_hex(curr_line);
            write_line(hex_value, 0, ob_output);
            init_string(curr_line);
            return;
        }
        if (inst->source_type == REGISTER_TYPE)
        {
            binary_value = int_to_binary_string(inst->source_value, 5);
            while (i < 5)
            {
                curr_line[i] = binary_value[i];
                i++;
            }
            write_line(curr_line, (*counter)++, object_output);
            hex_value = binary_to_hex(curr_line);
            write_line(hex_value, 0, ob_output);
            init_string(curr_line);
        }
        else
        {
            if (inst->source_type == LABEL_TYPE)
            {
                if (inst->source_value == 1)
                {
                    curr_line[11] = '1';
                }
                else
                {
                    curr_line[10] = '1';
                    binary_value = int_to_binary_string(inst->source_value, 10);
                    while (i < 10)
                    {
                        curr_line[i] = binary_value[i];
                        i++;
                    }
                }
            }
            else
            {
                binary_value = int_to_binary_string(inst->source_value, 10);
                while (i < 10)
                {
                    curr_line[i] = binary_value[i];
                    i++;
                }
            }
            write_line(curr_line, (*counter)++, object_output);
            hex_value = binary_to_hex(curr_line);
            write_line(hex_value, 0, ob_output);
            init_string(curr_line);
        }
        if (inst->dest_type == REGISTER_TYPE)
        {
            binary_value = int_to_binary_string(inst->source_value, 5);
            while (i < 5)
            {
                curr_line[i + 5] = binary_value[i];
                i++;
            }
            write_line(curr_line, (*counter)++, object_output);
            hex_value = binary_to_hex(curr_line);
            write_line(hex_value, 0, ob_output);
            init_string(curr_line);
        }
        else
        {
            if (inst->dest_type == LABEL_TYPE)
            {
                if (inst->dest_value == 1)
                {
                    curr_line[11] = '1';
                }
                else
                {
                    curr_line[10] = '1';
                    binary_value = int_to_binary_string(inst->dest_value, 10);
                    i = 0;
                    while (i < 10)
                    {
                        curr_line[i] = binary_value[i];
                        i++;
                    }
                }
            }
            else
            {

                binary_value = int_to_binary_string(inst->dest_value, 10);
                while (i < 10)
                {
                    curr_line[i] = binary_value[i];
                    i++;
                }
            }
            write_line(curr_line, (*counter)++, object_output);
            hex_value = binary_to_hex(curr_line);
            write_line(hex_value, 0, ob_output);
            init_string(curr_line);
        }
    }
}

/* write label data to file  */
void handle_label(Label *label, int *counter, FILE *object_output, FILE *ob_output)
{
    Word *word = NULL;
    int i = 0, j = 0;
    char *binary_value, *hex_value;
    char *curr_line;
    char *pointer;
    curr_line = (char *)malloc(OUTPUT_LINE_LENGTH * sizeof(char));
    init_string(curr_line);
    if (label->data_type)
    {
        if (label->data_type == DATA_DATA_TYPE)
        {
            word = get_next_word(label->data, 0);
            while (word)
            {
                binary_value = int_to_binary_string(atoi(word->str), 12);
                i = 0;
                while (i < 12)
                {
                    curr_line[i] = binary_value[i];
                    i++;
                }
                write_line(curr_line, (*counter)++, object_output);
                hex_value = binary_to_hex(curr_line);
                write_line(hex_value, 0, ob_output);
                init_string(curr_line);
                word = get_next_word(label->data, word->end_idx);
            }
        }
        else
        {
            pointer = label->data + 1; /* skip " */
            while (*pointer != '\n')
            {
                init_string(curr_line);
                if (*pointer == '"' || *pointer == '\"' || *pointer == '\n' || *pointer == '\t')
                {
                    write_line(curr_line, (*counter)++, object_output);
                    hex_value = binary_to_hex(curr_line);
                    write_line(hex_value, 0, ob_output);
                    break;
                }

                j = 0;
                binary_value = ascii_char_to_binary(*pointer);
                while (j < 8)
                {
                    curr_line[j + 4] = binary_value[j];
                    j++;
                }

                write_line(curr_line, (*counter)++, object_output);
                hex_value = binary_to_hex(curr_line);
                write_line(hex_value, 0, ob_output);
                pointer++;
                i++;
            }
        }
    }
    else if (label->code_flag)
    {
        handle_instruction(label->instruction, counter, object_output, ob_output);
    }
}

/* generate ent and ext files */
void generate_ent_ext_files(char *filename, InstructionTable *inst_table, LabelTable *table)
{
    FILE *ent_file = NULL; /* Write to ent file */
    FILE *ext_file = NULL; /* Write to ext file */
    Label *label, *iter_label;
    Instruction *inst;
    char *filename_ent;
    char *filename_ext;
    if (!table)
    {
        return;
    }
    filename_ent = (char *)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    filename_ext = (char *)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    strcpy(filename_ent, filename);
    strcpy(filename_ext, filename);
    strcat(filename_ent, ".ent"); /* Add file extension .ent to the original file name */
    strcat(filename_ext, ".ext"); /* Add file extension .ext to the original file name */
    label = table->head;
    while (label)
    {
        /* Print to ext file base & offset of external labels */
        if (label->external_flag)
        {
            if (inst_table)
            {
                inst = inst_table->head;
                while (inst)
                {
                    if (inst->source_operand && !strcmp(inst->source_operand, label->name))
                    {
                        if (!ext_file)
                        {
                            ext_file = fopen(filename_ext, "w");
                        }
                        fprintf(ext_file, "%s\t\t%d\n", label->name, inst->value + 1);
                    }
                    if (inst->dest_operand && !strcmp(inst->dest_operand, label->name))
                    {
                        if (!ext_file)
                        {
                            ext_file = fopen(filename_ext, "w");
                        }
                        if (inst->source_operand)
                        { /* has two operands */
                            fprintf(ext_file, "%s\t\t%d\n", label->name, inst->value + 2);
                        }
                        else
                        {
                            fprintf(ext_file, "%s\t\t%d\n", label->name, inst->value + 1);
                        }
                    }
                    inst = inst->next;
                }
            }

            iter_label = table->head;
            while (iter_label)
            {
                if (iter_label->instruction)
                {
                    inst = iter_label->instruction;
                    if (inst->source_operand && !strcmp(inst->source_operand, label->name))
                    {
                        if (!ext_file)
                        {
                            ext_file = fopen(filename_ext, "w");
                        }
                        fprintf(ext_file, "%s\t\t%d\n", label->name, inst->value + 1);
                    }
                    if (inst->dest_operand && !strcmp(inst->dest_operand, label->name))
                    {
                        if (!ext_file)
                        {
                            ext_file = fopen(filename_ext, "w");
                        }
                        if (inst->source_operand)
                        { /* has two operands */
                            fprintf(ext_file, "%s\t\t%d\n", label->name, inst->value + 2);
                        }
                        else
                        {
                            fprintf(ext_file, "%s\t\t%d\n", label->name, inst->value + 1);
                        }
                    }
                }
                iter_label = iter_label->next;
            }
        }

        /* Print to ent file line number of entry symbols */
        else if (label->entry_flag)
        {
            if (!ent_file)
            {
                ent_file = fopen(filename_ent, "w");
            }
            fprintf(ent_file, "%s,%d\n", label->name, label->value);
        }

        label = label->next;
    }
    free(filename_ent);
    free(filename_ext);
}

/* generate output files if had no errors in files */
void generate_object_files(char *filename, InstructionTable *inst_table, LabelTable *table)
{
    FILE *object_output, *ob_output;
    Instruction *inst = NULL;
    Label *label = NULL;
    char *object_filename_with_ext, *ob_filename_with_ext;
    int counter = 100; /* first address */
    inst = inst_table->head;
    label = table->head;
    object_filename_with_ext = (char *)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    ob_filename_with_ext = (char *)malloc(MAX_FILE_NAME_LENGTH * sizeof(char));
    strcpy(object_filename_with_ext, filename);
    strcat(object_filename_with_ext, ".object");
    object_output = fopen(object_filename_with_ext, "w");
    strcpy(ob_filename_with_ext, filename);
    strcat(ob_filename_with_ext, ".ob");
    ob_output = fopen(ob_filename_with_ext, "w");
    while (inst && label)
    {
        if (label->external_flag)
        {
            label = label->next;
            continue;
        }
        if (inst->value > label->value)
        {
            handle_label(label, &counter, object_output, ob_output);
            label = label->next;
        }
        else
        {
            handle_instruction(inst, &counter, object_output, ob_output);
            inst = inst->next;
        }
    }
    while (inst)
    {
        handle_instruction(inst, &counter, object_output, ob_output);
        inst = inst->next;
    }
    while (label)
    {
        if (label->external_flag)
        {
            label = label->next;
            continue;
        }
        handle_label(label, &counter, object_output, ob_output);
        label = label->next;
    }
    free(object_filename_with_ext);
    free(ob_filename_with_ext);
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
    Macro *current_macro = NULL;
    MacroList *list = NULL;
    FILE *input = NULL;
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
                current_macro = init_macro(get_nth_word(line, 2));
                if (!list)
                {
                    list = init_macro_list(current_macro);
                }
                else
                {
                    append_macro_to_list(list, current_macro);
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
int second_run(char *filename, char *filename_with_ext, InstructionTable *inst_table, LabelTable *table, int IC, int DC)
{
    Instruction *curr_inst;
    int errors_counter = 0;
    int value;
    Word *curr_word;
    Word *label_word;
    Label *curr_label;
    FILE *input;
    char *line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    input = fopen(filename_with_ext, "r");
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
            generate_object_files(filename, inst_table, table);
            generate_ent_ext_files(filename, inst_table, table);
            /* free(filename);*/
            /*free_labels(table);*/
            /* free_instructions(inst_table);*/
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
    fclose(input);
    /* free(line);*/
    /* free_word(curr_word);*/
    /* free_word(label_word);*/
    /*free_label(curr_label);*/
}

/* first run through the file all the instructions and labels validate them by schema provided */
int first_run(char *filename)
{
    int data_type, data_valid = INVALID, is_label_defining = OFF;
    int line_number = 1;
    int errors_count = 0;
    Word *curr_word;
    Word *label_word;
    InstructionTable *inst_table = NULL;
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
                data[strlen(data) - 1] = '\0';
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
                        current_label->data_type = data_type;
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
                data = (char *)malloc(sizeof(char) * (strlen(line) - curr_word->end_idx));
                strcpy(data, line + curr_word->end_idx + 1);
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
                    inst = init_inst();
                    L = validate_opcode(line + strlen(label_word->str), inst); /* validate instruction line and get number of lines or error code if invalid */
                    if (L < 0)
                    {                                         /* invalid instruction */
                        printf(" in line %d\n", line_number); /* add the line number to STDOUT */
                        errors_count++;
                        free_instruction(inst);
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
                inst = init_inst();
                L = validate_opcode(line, inst); /* validate instruction line and get number of lines or error code if invalid */
                if (L < 0)
                {                                         /* invalid instruction */
                    printf(" in line %d\n", line_number); /* add the line number to STDOUT */
                    errors_count++;
                    free_instruction(inst);
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
        if (current_label->data_type)
        {
            current_label->value = DC + IC;
            DC += get_data_length(current_label->data_type, current_label->data);
        }
        current_label = current_label->next;
    }

    if (!errors_count)
    {
        second_run(filename, input_filename_with_ext, inst_table, table, IC, DC);
    }
    else
    {
        printf("had errors in first run on file.\n");
        return ERROR_CODE;
    }
    free(line);
    /* free_word(curr_word);*/
    /* free_word(label_word);*/
    /* free_label(current_label);*/
    return NO_ERROR_CODE;
}
