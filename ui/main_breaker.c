#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "../tools/breaker.h" 

const int PARS = 4;

// ./break storage/rt_6_a_1000_5/ tests/hashes 4

void main (argc, argv)
int argc;
char *argv[];
{
	if(argc < PARS){
		printf("No ha introducido todos los parametros\n");
		exit(1);
	}	

	char loc[80];
	char hfile[80];
	int thrds;

	strcpy(loc,argv[1]);
	strcpy(hfile,argv[2]);
	thrds = atoi(argv[3]);

	printf("location: %s\n",loc);
	printf("hash file: %s\n",hfile);
	printf("threads: %d\n",thrds);

	break_down(loc, hfile, thrds);
}
