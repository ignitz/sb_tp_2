#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

Tabela_Opcodes opcode_table[MAX_OPCODE] = {
	// name, value, size
	{"MOV", 1, 6},
	{"ADD", 2, 6},
	{"SUB", 3, 6},
	{"MUL", 4, 4},
	{"DIV", 5, 4},
	{"AND", 6, 6},
	{"NOT", 7, 4},
	{"OR", 8, 6},
	{"CMP", 9, 6},
	{"JMP", 10, 4},
	{"JZ", 11, 4},
	{"JS", 12, 4},
	{"CALL", 13, 4},
	{"RET", 14, 2},
	{"PUSH", 15, 4},
	{"POP", 16, 4},
	{"DUMP", 17, 2},
	{"READ", 18, 4},
	{"WRITE", 19, 4},
	{"HLT", 20, 2}
};

// Inicializa a tabela de simbolos
void initialize_symbol_table()
{
	int i;
	for(i=0; i<1000; i++){
		symbol_table[i].address = 0;
		symbol_table[i].in_use = 0;
		symbol_table[i].file = 0;
	}
}

// Insere um simbolo na tabela
void insert_symbol(char *name, int address, int file)
{
	int i = 0;
	while(symbol_table[i].in_use) i++;
	strcpy(symbol_table[i].name, name);
	symbol_table[i].address = address;
	symbol_table[i].file = file;
	symbol_table[i].in_use = 1;
}

// Retorna o endereco de um simbolo da tabela
int get_symbol_address(char *name)
{
	int i = 0;
	while(i<1000 && strcmp(symbol_table[i].name, name)!=0) i++;
	return (i==1000) ? -1 : symbol_table[i].address;
}

// Converte String HEX em Int
// 0xA --> 10
int get_hex_value(char *hexString)
{
	if (strlen(hexString) > 2)
		return (int) strtol(hexString, NULL, 0);
	else
		return 0;
}

// Retorna um parametro de alguma instrucao da tabela de instrucoes
// Recebe como entrada o nome do opcode
int get_opcode(char *name)
{
	int i;
	for(i = 0; i < MAX_OPCODE; i++){
		if(strcmp(opcode_table[i].name, name) == 0){
			return opcode_table[i].value;
		}
	}
	return -1;
}

// Retorna o tamanho do dado do opcode
// 16, 32 ou 48 bits
int get_size(char *name)
{
	int i;
	for(i = 0; i < MAX_OPCODE; i++){
		if(strcmp(opcode_table[i].name, name) == 0){
			return opcode_table[i].size;
		}
	}
	return -1;
}

// Retorna a proxima linha de um stream
char *get_next_line(FILE *f)
{
	int cont = 0, max = 32;
	char *line = (char *)malloc(max*sizeof(char));
	char c;
	while(1){
		c = fgetc(f);
		if(c==EOF || c=='\n'){
			break;
		}
		if(c=='\r'){ // Quebra de linhas do Windows
			fgetc(f);
			break;
		}
		if(cont==(max-1)){
			line = (char *)realloc(line, (2*max)*sizeof(char));
			max = 2*max;
		}
		line[cont] = c;
		cont++;
	}
	line[cont] = '\0';
	if(c==EOF && cont==0){
		free(line);
		return NULL;
	}
	return line;
}