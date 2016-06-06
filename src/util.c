#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

// Tabela de instrucoes
Tabela_Opcodes opcode_table[25] = {
	{"LOAD", 1, 2, 0}, {"STORE", 2, 2, 0}, {"READ",  3, 1, 0}, {"WRITE", 4, 1, 0},
	{"COPY", 5, 2, 1}, {"XCH",   6, 2, 1}, {"ADD",   7, 2, 1}, {"SUB",   8, 2, 1},
	{"AND",  9, 2, 1}, {"OR",   10, 2, 1}, {"XOR",  11, 2, 1}, {"NOT",  12, 1, 0},
	{"JMP", 13, 2, 0}, {"JZ",   14, 2, 0}, {"JNZ",  15, 2, 0}, {"JN",   16, 2, 0},
	{"JNN", 17, 2, 0}, {"PUSH", 18, 1, 0}, {"POP",  19, 1, 0}, {"CALL", 20, 2, 0},
	{"RET", 21, 1, 0}, {"DUMP", 22, 1, 0}, {"HALT", 23, 1, 0}, {"WORD", 24, 2, 0},
	{"END", 25, 1, 0}
};

// Retorna um parametro de alguma instrucao da tabela de instrucoes
// Recebe como entrada o ID do opcode
int get_opcode_param_by_id_by_id(int id, int p)
{
	int i;
	for(i=0; i<25; i++){
		if(opcode_table[i].v==id){
			return (p==0) ? opcode_table[i].v : (p==1) ? opcode_table[i].s : (p==2) ? opcode_table[i].r : -1;
		}
	}
	return -1;
}

// Retorna um parametro de alguma instrucao da tabela de instrucoes
// Recebe como entrada o nome do opcode
int get_opcode_param_by_name(char *name, int p)
{
	int i;
	for(i=0; i<25; i++){
		if(strcmp(opcode_table[i].name, name)==0){
			return (p==0) ? opcode_table[i].v : (p==1) ? opcode_table[i].s : (p==2) ? opcode_table[i].r : -1;
		}
	}
	return -1;
}

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

// Retorna o id do arquivo de um simbolo da tabela
int get_symbol_file(char *name)
{
	int i = 0;
	while(i<1000 && strcmp(symbol_table[i].name, name)!=0) i++;
	return (i==1000) ? -1 : symbol_table[i].file;
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