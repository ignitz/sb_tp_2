#ifndef util_H_INCLUDED
#define util_H_INCLUDED

#define MAX_SYMBOL 1000
#define MAX_OPCODE 21

#define CHAR_IGNORE "\t ,"

// Tabela de simbolos
struct {
	char name[16];
	int address;
	int in_use;
} symbol_table[MAX_SYMBOL];

// Tabela de instrucoes
typedef struct {
	char name[8];
	int value; // Valor do opcode
	int size; // Quantidade de bytes da instrucao
} Tabela_Opcodes;

void initialize_symbol_table();
void insert_symbol(char *name, int address);
int get_symbol_address(char *name);
int get_hex_value(char *name);
int get_opcode(char *name);
int get_size(char *name);
char *get_next_line(FILE *f);

#endif
