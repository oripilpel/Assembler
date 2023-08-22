#ifndef ASSEMBLER_OPERATION_H
#define ASSEMBLER_OPERATION_H

typedef struct InstructionTable
{
    Instruction *head; /* the first instruction in linked list */
} InstructionTable;

int validate_opcode(char *line, Instruction *inst);

void append_instruction(InstructionTable *table, Instruction *inst);

int get_operand_address(Instruction *inst, LabelTable *table, int type);

Instruction *init_inst();

InstructionTable *init_inst_table(InstructionTable *table);

#endif
