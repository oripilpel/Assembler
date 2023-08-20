#ifndef ASSEMBLER_OPERATION_H
#define ASSEMBLER_OPERATION_H

typedef Instruction
{
    struct Instruction *next;
    int value;            /* the address in the memory */
    char *opname;         /* instruction name */
    char *source_operand; /* source operand */
    char *dest_operand;   /* destination operand */
    char *opcode;         /* instruction code */
    int source_value;     /* source address in memory */
    int dest_value;       /* destination address in memory */
    int source_type;      /* source operand type */
    int dest_type;        /* destination operand type */
}
Instruction;

typedef struct InstructionTable
{
    Instruction *head; /* the first instruction in linked list */
} InstructionTable;

int validate_opcode(char *line);

void append_instruction(InstructionTable *table, Instruction *inst);

#endif
