#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

// Primeira passada do montador
void pass_one(FILE *entrada)
{
	
}

// Segunda passada do montador
void pass_two(FILE *entrada, FILE *objeto)
{
	fseek(entrada, 0, SEEK_SET);
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
