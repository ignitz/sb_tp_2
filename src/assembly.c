#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

// Primeira passada do montador
void pass_one(FILE *entrada)
{
	initialize_symbol_table();
	int location_counter = 0;
	char *line;
	while((line = get_next_line(entrada))){
		char *token = strtok(line, "\t ");
		while(token){
			if(token[0] == ';'){ // Comentario, ignora resto da linha
				break;
			}
			if(token[strlen(token)-1] == ':'){ // Label, insere na tabela de simbolos
				token[strlen(token)-1] = '\0';
				insert_symbol(token, location_counter, 0);
			} else if(get_opcode_param_by_name(token, 0) > 0){ // Verifica se eh opcode
				location_counter += (get_opcode_param_by_name(token, 0)==24) ? 1 : get_opcode_param_by_name(token, 1);
			}
			token = strtok(NULL, "\t ");
		}
		free(line);
	}
}

// Segunda passada do montador
void pass_two(FILE *entrada, FILE *objeto)
{
	fseek(entrada, 0, SEEK_SET);

	int end = 0;
	while(!end){
		char *line = get_next_line(entrada);
		char *token = strtok(line, "\t ");
		while(token && !end){
			if(token[0] == ';'){ // Comentario, ignora resto da linha
				break;
			}
			if(token[strlen(token)-1] != ':'){ // Se nao eh label
				int opcode = get_opcode_param_by_name(token, 0);
				if(opcode == 24){
					token = strtok(NULL, "\t ");
					fprintf(objeto, "%i ", 24);
					fprintf(objeto, "%i ", atoi(token));
				} else if(opcode == 25){ // END
					end = 1;
					break;
				} else { // Instrução normal
					fprintf(objeto, "%i ", opcode);
					if(get_opcode_param_by_name(token, 1) == 2){ // Tem parametro
						int is_reg = (get_opcode_param_by_name(token, 2)==1);
						token = strtok(NULL, "\t ");
						if(is_reg){
							fprintf(objeto, "%i ", atoi(&token[1]));
						} else {
							fprintf(objeto, "%s ", token);
						}
					}
				}
			}
			token = strtok(NULL, "\t ");
		}
		free(line);
	}

	// Imprime a tabela de símbolos no arquivo objeto
	fprintf(objeto, "\n");
	int i = 0;
	while(symbol_table[i].in_use){
		fprintf(objeto, "%s %i ", symbol_table[i].name, symbol_table[i].address);
		i++;
	}
}

int main(int argc, char *argv[])
{
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
	FILE *objeto = fopen(s, "w+");

	pass_one(entrada); // Passada 1 para definicao da tabela de simbolos
	pass_two(entrada, objeto); // Codificacao com enderecos descomplicados

	fclose(entrada);
	fclose(objeto);

	if(verbose){ // Impressão de tabela de simbolos, caso seja modo verbose
		printf("Tabela de simbolos:\n");
		int i = 0;
		while(symbol_table[i].in_use){
			printf("%s %i\n", symbol_table[i].name, symbol_table[i].address);
			i++;
		}
	}

	return 0;
}
