#ifndef util_H_INCLUDED
#define util_H_INCLUDED

// Tabela de instrucoes
typedef struct {
	char name[8];
	int v; // Valor do opcode
	int s; // Quantidade de bytes da instrucao
	int r; // Se operando da inst eh reg ou nao
} Tabela_Opcodes;

// Tabela de simbolos, alocada com 1000 posicoes
struct {
	char name[16];
	int address;
	int in_use;
	int file;
} symbol_table[1000];

// int get_opcode_param_by_id_by_id(int id, int p);
// int get_opcode_param_by_name(char *name, int p);
// int get_symbol_address(char *name);
// int get_symbol_file(char *name);
// void initialize_symbol_table();
// void insert_symbol(char *name, int address, int file);
char *get_next_line(FILE *f);

#endif
