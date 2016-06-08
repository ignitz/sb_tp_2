#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#define DEBUG

// Primeira passada do montador
void pass_one(FILE *entrada) {

	initialize_symbol_table();
	int location_counter = 2; // Posição de memória
	char *line;
	while(line = get_next_line(entrada)) {
		char *token = strtok(line, " ,");
		while(token) {
			if (token[0] == ';') { // Ignora comentário
				break;
			}
			if (token[strlen(token)-1] == ':') { // É label
				// Insere label na tabela de símbolos
				token[strlen(token)-1] = '\0';
				#ifdef DEBUG
					printf("\n%s: ", token);
				#endif
				insert_symbol(token, location_counter, 0);
			} else if (get_opcode(token) > 0){ // Verifica se é Opcode
				#ifdef DEBUG
					printf("\n%s 0x%X %d", token, get_opcode(token), get_size(token));
					location_counter += get_size(token);
				#endif
			} else { // Operandos
				#ifdef DEBUG
					printf(" .%s ", token);
				#endif
			}
			token = strtok(NULL, " ,");
		}
		free(line);
	}
	#ifdef DEBUG
		printf("\nlocation_counter = 0x%X\n", location_counter);
	#endif
}

// Segunda passada do montador
void pass_two(FILE *entrada, FILE *objeto) {
}

int main(int argc, char *argv[]) {
	int i;
	int verbose = 0; // Flags para modo verbose e definição de saida
	int saida = 0;	
	char *e = argv[1]; // Definicao do arquivo de entrada
	char *s;
	
	if(argc < 2){
		printf("Faca a chamada com o nome do arquivo .asm\n");
		return 0;
	}

	for(i = 2; i < argc; i++){ 
		// Verifica se eh verbose
		if(strcmp(argv[i], "-v") == 0){
			verbose = 1;
		}
		// Verifica se nome de saida foi passado
		if(strcmp(argv[i], "-o")==0){
			saida = 1;
			s = argv[i+1];
		}
	}
	
	if(saida == 0){
		s = "out.sa"; // Nome padrão para geracao de executavel
	}

	FILE *entrada = fopen(e, "r");
	if (entrada == NULL) {
		printf("Erro ao abrir arquivo\n");
		return -1;
	}
	FILE *objeto = fopen(s, "w+");

	pass_one(entrada); // Passada 1 para definicao da tabela de simbolos
	pass_two(entrada, objeto); // Codificacao com enderecos descomplicados

	fclose(entrada);
	fclose(objeto);

	if(verbose){ // Impressão de tabela de simbolos, caso seja modo verbose
		printf("Tabela de simbolos:\n");
		int i = 0;
		while(symbol_table[i].in_use){
			printf("%s 0x%X\n", symbol_table[i].name, symbol_table[i].address);
			i++;
		}
	}

	return 0;
}
