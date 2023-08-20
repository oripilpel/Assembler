#include "constants.h"
#include "label.h"
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef Instruction
{
    int value;
    char *opname;
    char *source_operand;
    char *dest_operand;
    char *opcode;
    int source_value;
    int dest_value;
    int source_type;
    int dest_type;
}
Instruction;

typedef struct InstructionTable
{
    Instruction *head;
} InstructionTable;

typedef struct Operation
{
    char *opname;
    int num_of_operands;
    int source_allowed;
    int dest_allowed;
    char *opcode;
} Operation;

/* append operation to the end of operation table */
void append_instruction(InstructionTable *table, Instruction *inst)
{
    Instruction *current;
    if (!table) /* if table hasen't initialized yet */
    {
        table = (InstructionTable *)malloc(sizeof(InstructionTable));
        table->head = inst;
        return;
    }
    *current = table->head;
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
    case IMMEDIATE_TYPE:     /* 1 */
        return allowed % 10; /* return the reminder */
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
    int num_of_lines = 0;
    char *opname;
    char *first_operand;
    char *second_operand;
    Operation *curr_op = NULL;
    Word *curr_word = get_next_word(line, 0);
    opname = curr_word->str;
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
        printf("ERRROR - no operation name %s", opname);
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
    first_operand = curr_word->str;
    curr_word = get_next_word(line, curr_word->end_idx);
    second_operand = curr_word->str;

    /* validate the first operand */
    if (num_of_operands)
    {
        op_type = get_operand_type(first_operand);
        switch (op_type)
        {

        case REGISTER_TYPE:
            first_operand++; /* skip @ */
            /* make sure the r */
            if (*first_operand != 'r')
            {
                printf("ERROR - register name %s didn't start with r", first_operand);
                return ERROR_CODE;
            }
            first_operand++;              /* skip r */
            if (first_operand[1] != '\0') /* number is not between 0-9 */
            {
                printf("ERROR - register number is invalid %s", first_operand);
                return ERROR_CODE;
            }
            num = first_operand[0] - '0'; /* convert the char to numeric */
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
        }
    }

    /* validate the second operand */
    if (num_of_operands == 2)
    {
        op_type = get_operand_type(second_operand);
        if ()
            switch (op_type)
            {

            case REGISTER_TYPE:
                second_operand++; /* skip @ */
                /* make sure the r */
                if (*second_operand != 'r')
                {
                    printf("ERROR - register name %s didn't start with r", second_operand);
                    return ERROR_CODE;
                }
                second_operand++;              /* skip r */
                if (second_operand[1] != '\0') /* number is not between 0-9 */
                {
                    printf("ERROR - register number is invalid %s", second_operand);
                    return ERROR_CODE;
                }
                num = second_operand[0] - '0'; /* convert the char to numeric */
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
        case IMMEDIATE_TYPE:
            return atoi(inst->dest_operand);
        case LABEL_TYPE:
            l = find_label(inst->dest_operand);
            if (!l)
            {
                printf("ERROR - no label found with name %s\n", inst->dest_operand);
                return ERROR_CODE;
            }
            else
            {
                return l.value;
            }
        }
    }
    else
    {
        switch (inst->source_type)
        {
        case REGISTER_TYPE: /* the address is the register's number */
            return atoi(inst->source_operand + 2);
        case IMMEDIATE_TYPE:
            return atoi(inst->source_operand);
        case LABEL_TYPE:
            l = find_label(inst->source_operand);
            if (!l)
            {
                printf("ERROR - no label found with name %s\n", inst->source_operand);
                return ERROR_CODE;
            }
            else
            {
                return l.value;
            }
        }
    }
}