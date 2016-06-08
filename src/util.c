#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

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