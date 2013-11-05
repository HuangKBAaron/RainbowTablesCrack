#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "../tools/generator.h"


const int PARS = 12;
const char PAR1[3] = "-l";
const char PAR2[4] = "-cl";
const char PAR3[3] = "-t";
const char PAR4[4] = "-tl";
const char PAR5[5] = "-ths";

const char DOM1[3] = "-a";
const char DOM2[3] = "-A";
const char DOM3[3] = "-0";
const char DOM4[4] = "-aA";
const char DOM5[4] = "-a0";
const char DOM6[4] = "-A0";
const char DOM7[5] = "-aA0";


// ./generate -l 6 -a -cl 1000 -t 4 -tl 10000 -ths 8

/*
Longitud: 6
Iteraciones: 5000
Longitud de tabla: 40000 
Numero de tablas: 4
*/




void main (argc, argv)
int argc;
char *argv[];
{
	if(argc < PARS){
		printf("No ha introducido todos los parametros\n");
		exit(1);
	}	

	unsigned int k_length ;
	unsigned int ch_length ;
	unsigned int t_length ;
	int n_tables ;
	int threads ;
	char dom[5];


	if(strcmp(argv[1], PAR1) == 0){
		k_length = atoi(argv[2]);
		printf("kl %d\n",k_length);
	}else{
		printf("Error al introducir el paramentro %s. %s no es correcto.\n",PAR1,argv[1]);
	}

	if( (strcmp(argv[3], DOM1) == 0) || (strcmp(argv[3], DOM2) == 0) || (strcmp(argv[3], DOM3) == 0) ||
	(strcmp(argv[3], DOM4) == 0) || (strcmp(argv[3], DOM5) == 0) || (strcmp(argv[3], DOM5) == 0) ||
	(strcmp(argv[3], DOM7) == 0) ){
		strcpy(dom, argv[3]);
		printf("dom %s\n",dom);
	}else{
		printf("Error al introducir el dominio\n");
		exit(1);
	}

	if(strcmp(argv[4], PAR2) == 0){
		ch_length = atoi(argv[5]);
		printf("chl %d\n",ch_length);
	}else{
		printf("Error al introducir el paramentro %s\n",PAR2);
		exit(1);
	}
	
	if(strcmp(argv[6], PAR3) == 0){
		n_tables = atoi(argv[7]);
		printf("t %d\n",n_tables);
	}else{
		printf("Error al introducir el paramentro %s\n",PAR3);
		exit(1);
	}

	if(strcmp(argv[8], PAR4) == 0){
		t_length = atoi(argv[9]);
		printf("tl %d\n",t_length);
	}else{
		printf("Error al introducir el paramentro %s. %s no es correcto.\n",PAR4,argv[8]);
		exit(1);
	}

	if(strcmp(argv[10], PAR5) == 0){
		threads = atoi(argv[11]);
		printf("ths %d\n",threads);
	}else{
		printf("Error al introducir el paramentro %s. %s no es correcto.\n",PAR5,argv[10]);
		exit(1);
	}


	generate_tables(k_length, &dom[1], t_length, ch_length, n_tables, threads);
}
