#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#define DEBUG

// Primeira passada do montador
void pass_one(FILE *entrada) {

	initialize_symbol_table();
	int location_counter = 2; // Posição de memória
	char *line, *token;
	while(line = get_next_line(entrada)) {
		token = strtok(line, CHAR_IGNORE);
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
				#endif

				location_counter += get_size(token);
			} else { // Operandos
				
				#ifdef DEBUG
					printf(" .%s ", token);
				#endif

			}
			token = strtok(NULL, CHAR_IGNORE);
		}
		free(line);
	}

	#ifdef DEBUG
		printf("\nlocation_counter = 0x%X\n", location_counter);
	#endif
}

// Segunda passada do montador
// Varre de forma semelhante na primeira passada
void pass_two(FILE *entrada, FILE *objeto) {
	fseek(entrada, 0, SEEK_SET);

	char *line, *token;
	while(line = get_next_line(entrada)) {
		token = strtok(line, CHAR_IGNORE);
		while(token) {
			if (token[0] == ';') {
				break;
			}
			if (token[strlen(token)-1] == ':') {
				// Ignora label
			} else if (get_opcode(token) > 0) {
				// Aqui faço a análise da linha
				switch(get_size(token)) {
					case 2: // 16 bits
						printf("Case 2 %s\n", token);
						break;
					case 4:
						printf("Case 4 %s", token);
						token = strtok(NULL, CHAR_IGNORE);
						printf(" %s\n", token);
						break;
					case 6:
						printf("Case 6 %s", token);
						token = strtok(NULL, CHAR_IGNORE);
						printf(" %s ", token);
						token = strtok(NULL, CHAR_IGNORE);
						printf("%s\n", token);
						break;
					default:
						printf("Exceção:\n");
						printf("token = %s\n", token);
						exit(0);
						break;
				}
			} else { // Caso ocorra algo inexpedado
				printf("Exceção:\n");
				printf("token = %s\n", token);
				exit(0);
				
			}

			token = strtok(NULL, CHAR_IGNORE);
		}
		free(line);
	}
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
		if(strcmp(argv[i], "-o") == 0){
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
