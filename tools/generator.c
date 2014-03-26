#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/sha.h>

#include "generator.h"
#include "../devices/reduction.h"
#include "../lib/hashTable3.h"
#include "../lib/spclib.h"
#include "../lib/space.h"


static const unsigned int THREADS_NUMBER = 8;
static const char *RB_TABLES_PATH = "storage/";

static const char *DIRECTORY;		// GLOBAL VAR


static unsigned int table_length;
static unsigned int chain_length;
static unsigned int ntables;

// global vars
static unsigned long long collisions;
static unsigned int chains;
static unsigned int i_index;
static unsigned int num_table;			

static Mmp_Hash hash_table;


sem_t  sem;  /* Semaforo */
sem_t  sem2;  /* Semaforo 2 */




static const char *name_directory(int key_length, char *domain_tag, unsigned int ch_length, int tables);
static unsigned int generate_table(int n_table);
static unsigned long long generate_chain(unsigned long long indexInicial, int tabla);
static void *child(void *v);




static const char * 
name_directory(unsigned int key_length, char *charset_tag, unsigned int chain_length, unsigned int tables){

    char kl[3];
    char cl[10];
    char nt[3];

    itoa(key_length,kl);
    itoa(chain_length,cl);
    itoa(tables,nt);

    char *str = malloc(strlen(RB_TABLE_PATH) + strlen(RB_TABLE_NAME) + 1 + strlen(kl) + 1 + strlen(charset_tag) + 1 + strlen(cl) + 1 + strlen(nt) + 1);

    strcpy(str, RB_TABLE_PATH);
    strncat(str, RB_TABLE_NAME, strlen(RB_TABLE_NAME));
    strncat(str, "_", 1);
    strncat(str, kl, strlen(kl));
    strncat(str, "_", 1);
    strncat(str, charset_tag, strlen(charset_tag));
    strncat(str, "_", 1);
    strncat(str, cl, strlen(cl));
    strncat(str, "_", 1);
    strncat(str, nt, strlen(nt));
    strncat(str, "/", 1);

    return str;

}

void 
init_generator(unsigned int k_length, char *cs_tag, unsigned int t_length, unsigned int ch_length, unsigned int tables){

	if(sem_init(&sem, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	if(sem_init(&sem2, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	init_reduction(k_length, d_tag);

	DIRECTORY = name_directory(k_length, cs_tag, ch_length, tables);

	if(mkdir(DIRECTORY,S_IRWXU) !=0){
		printf("Error al crear la carpeta\n");
	}

	i_index = 0;
	collisions = 0 ;

	table_length = t_length;
	chain_length = ch_length;
	ntables = tables;

}

static const char *
name_table(unsigned int table){

    char nt[3];

    itoa(table, nt);

    char *str = malloc(strlen(DIRECTORY) + strlen(nt));
    strncat(str, DIRECTORY, strlen(DIRECTORY));
    strncat(str, nt, strlen(nt));

    return str;
}

static pthread_t
newproc(void *(*tmain)(void *), void *args)
{
	pthread_t thread;
 	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
 	if(pthread_create(&thread, &attr, tmain, args)){
		fprintf(stderr, "cannot create pthread\n");
		exit(EXIT_FAILURE);
	}
	return thread;
}

static void *
child(void *v)
{
	unsigned long long index_f ;
	unsigned int index_0 ;

	sem_wait(&sem);	// down()
	while(chains < table_length){
		sem_post(&sem);	// up()

		sem_wait(&sem2);	// down()
			i_index++ ;
			index_0 = i_index ;
		sem_post(&sem2);	// up()

		index_f = generate_chain(index_0,num_table);

		sem_wait(&sem);	// down()
			if(get3(&hash_table,index_f)){
				collisions++;
			}else{
				put3(&hash_table,index_f,index_0);
				chains++;
			}
	}
	sem_post(&sem);	// up()


	pthread_exit(0);
}

static unsigned int 
generate_table(unsigned int n_table)
{
	num_table =  n_table;
	chains = 0 ;

	char *name = name_table(t);

	create_hash_table3(&hash_table, name);

	pthread_t  *childs;
	childs = malloc(THREADS_NUMBER * sizeof(pthread_t));

	/*
	 * crea todos los threads
	 */
	int i;
	for(i = 0; i < THREADS_NUMBER ; i++){
		childs[i] = newproc(child, NULL);
	}
	/*
	 * espera a que acaben todos los threads
	 */
	for(i= 0; i <  THREADS_NUMBER; i++)
		pthread_join(childs[i], NULL);
	free(childs);

	close_hash_table3(&hash_table);

	printf("Colisiones: %u\n",collisions);

	return i_index;	
}

static unsigned long long
generate_chain(unsigned long long indexInicial, unsigned int tabla)
{
	char r[MAX_KEY_LENGTH+1];
	unsigned char sha[20];

	unsigned long long index;
	index = indexInicial;

	int i;
	for(i = 0; i < chain_length ; i++){
		index2plain(index,r);
                SHA1(r, strlen(r), sha);	
		index = sha2index(sha, i, tabla);
	}

	return index;
}

void 
generate_rainbow_tables()
{
	int i ;
	for(i = 0 ; i < ntables ; i++){
		generate_table(i);
	}
}


