#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

//#define DEBUG

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
				
				// TODO Implementar variável na memória
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
void pass_two(FILE *entrada, FILE *objeto, int posicaoInicial) {
	fseek(entrada, 0, SEEK_SET);
	
	char *line, *token;
	int buffer,
		operando1, operando2;

	// 0  Nenhum operando 
	// 1  Registrador 
	// 2  Memória 
	// 3  Registrador e memória 
	// 4  Memória e Registrador 
	// 5  Registrador e Registrador 
	// 6  Memória e imediato (número)
	// 7  Registrador e imediato (número) 
	// 8  Imediato (número) 
	int flagOperando; // tipos de Operando

	// Insere a posição inicial do programa
	fwrite(&posicaoInicial, 2, 1, objeto);

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

						buffer = get_opcode(token) << 8;
						flagOperando = 0;
						break;
					case 4: // 32 bits
				
						buffer = get_opcode(token) << 8;
						token = strtok(NULL, CHAR_IGNORE);

						// Falta implementar acesso a memória por variável
						if (strcmp(token, "AL") == 0) {
							operando1 = 0;
							flagOperando = 1;
						} else if (strcmp(token, "AH") == 0) {
							operando1 = 1;
							flagOperando = 1;
						} else if (strcmp(token, "AX") == 0) {
							operando1 = 2;
							flagOperando = 1;
						} else if (strcmp(token, "BH") == 0) {
							operando1 = 3;
							flagOperando = 1;
						} else if (strcmp(token, "BL") == 0) {
							operando1 = 4;
							flagOperando = 1;
						} else if (strcmp(token, "BX") == 0) {
							operando1 = 5;
							flagOperando = 1;
						} else if (strcmp(token, "CL") == 0) {
							operando1 = 6;
							flagOperando = 1;
						} else if (strcmp(token, "CH") == 0) {
							operando1 = 7;
							flagOperando = 1;
						} else if (strcmp(token, "CX") == 0) {
							operando1 = 8;
							flagOperando = 1;
						} else if (token[0] == '_') {
							operando1 = get_symbol_address(token);
							flagOperando = 8;
						} else if (token[0] == '0' && token[1] == 'x') {
							operando1 = get_hex_value(token);
							flagOperando = 8;
						}

						buffer += flagOperando;
						break;
					case 6: // 48 bits

						buffer = get_opcode(token) << 8;

						token = strtok(NULL, CHAR_IGNORE);

						// Falta implementar acesso a memória por variável
						if (strcmp(token, "AL") == 0) {
							operando1 = 0;
							flagOperando = 1;
						} else if (strcmp(token, "AH") == 0) {
							operando1 = 1;
							flagOperando = 1;
						} else if (strcmp(token, "AX") == 0) {
							operando1 = 2;
							flagOperando = 1;
						} else if (strcmp(token, "BH") == 0) {
							operando1 = 3;
							flagOperando = 1;
						} else if (strcmp(token, "BL") == 0) {
							operando1 = 4;
							flagOperando = 1;
						} else if (strcmp(token, "BX") == 0) {
							operando1 = 5;
							flagOperando = 1;
						} else if (strcmp(token, "CL") == 0) {
							operando1 = 6;
							flagOperando = 1;
						} else if (strcmp(token, "CH") == 0) {
							operando1 = 7;
							flagOperando = 1;
						} else if (strcmp(token, "CX") == 0) {
							operando1 = 8;
							flagOperando = 1;
						}

						token = strtok(NULL, CHAR_IGNORE);

						// 3  Registrador e memória 
						// 4  Memória e Registrador 
						// 5  Registrador e Registrador 
						// 6  Memória e imediato (número)
						// 7  Registrador e imediato (número) 

						// Falta implementar acesso a memória por variável
						if (strcmp(token, "AL") == 0) {
							operando2 = 0;
							if (flagOperando == 1) flagOperando = 5; else if (flagOperando == 2) flagOperando = 4; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (strcmp(token, "AH") == 0) {
							operando2 = 1;
							if (flagOperando == 1) flagOperando = 5; else if (flagOperando == 2) flagOperando = 4; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (strcmp(token, "AX") == 0) {
							operando2 = 2;
							if (flagOperando == 1) flagOperando = 5; else if (flagOperando == 2) flagOperando = 4; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (strcmp(token, "BH") == 0) {
							operando2 = 3;
							if (flagOperando == 1) flagOperando = 5; else if (flagOperando == 2) flagOperando = 4; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (strcmp(token, "BL") == 0) {
							operando2 = 4;
							if (flagOperando == 1) flagOperando = 5; else if (flagOperando == 2) flagOperando = 4; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (strcmp(token, "BX") == 0) {
							operando2 = 5;
							if (flagOperando == 1) flagOperando = 5; else if (flagOperando == 2) flagOperando = 4; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (strcmp(token, "CL") == 0) {
							operando2 = 6;
							if (flagOperando == 1) flagOperando = 5; else if (flagOperando == 2) flagOperando = 4; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (strcmp(token, "CH") == 0) {
							operando2 = 7;
							if (flagOperando == 1) flagOperando = 5; else if (flagOperando == 2) flagOperando = 4; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (strcmp(token, "CX") == 0) {
							operando2 = 8;
							if (flagOperando == 1) flagOperando = 5; else if (flagOperando == 2) flagOperando = 4; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (token[0] == '_') {
							operando2 = get_symbol_address(token);
							if (flagOperando == 1) flagOperando = 7; else if (flagOperando == 2) flagOperando = 6; else {printf("Erro no tipo de operandos\n"); exit(0);}
						} else if (token[0] == '0' && token[1] == 'x') {
							operando2 = get_hex_value(token);
							if (flagOperando == 1) flagOperando = 7; else if (flagOperando == 2) flagOperando = 6; else {printf("Erro no tipo de operandos\n"); exit(0);}
						}

						buffer += flagOperando;
						break;
					default:
						printf("Exceção:\n");
						printf("token = %s\n", token);
						fclose(objeto);
						fclose(entrada);
						exit(0);
						break;
				}

				// Passo da escrita no arquivo
				switch (flagOperando) {
					case 0: // 0  Nenhum operando
						fwrite(&buffer, 2, 1, objeto);
						break;

					case 1: // 1  Registrador
					case 2: // 2  Memória
					case 8: // 8  Imediato (número)
						fwrite(&buffer, 2, 1, objeto);
						fwrite(&operando1, 2, 1, objeto);
						break;

					case 3: // 3  Registrador e memória 
					case 4: // 4  Memória e Registrador 
					case 5: // 5  Registrador e Registrador 
					case 6: // 6  Memória e imediato (número)
					case 7: // 7  Registrador e imediato (número)
						fwrite(&buffer, 2, 1, objeto);
						fwrite(&operando1, 2, 1, objeto);
						fwrite(&operando2, 2, 1, objeto);
						break;

					default: // Se der algum problema
						printf("Erro em retorno de tipo de operando\n");
						break;
				}

			} else { // Caso ocorra algo inexpedado
				printf("Exceção:\n");
				printf("token = %s\n", token);
				fclose(objeto);
				fclose(entrada);
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
	pass_two(entrada, objeto, 0); // Codificacao com enderecos descomplicados

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
