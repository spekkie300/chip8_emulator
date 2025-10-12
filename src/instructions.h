#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

void fetch_instruction(void);
void exec_instructions(void);
void instr_null(void);
void instr_00E0(void);
void instr_1NNN(void);
void instr_6XNN(void);
void instr_7XNN(void);
void instr_ANNN(void);
void instr_DXYN(void);
#endif
