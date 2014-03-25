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


const unsigned int THREADS_NUMBER = 8;
const char *RB_TABLES_PATH = "storage/";

static const char *directory;


static unsigned int table_length;
static unsigned int chain_length;
static unsigned int ntables;

static unsigned long long collisions;
static unsigned int chains;
static unsigned int i_index;
static unsigned int num_table;			// global var

static Mmp_Hash hash_table;


sem_t  sem;  /* Semaforo */
sem_t  sem2;  /* Semaforo 2 */




static void init_table_generator(int t);
static const char *name_directory(int key_length, char *domain_tag, unsigned int ch_length, int tables);
static void name_table(char *name, int table);
static unsigned int generate_table(int n_table);
static unsigned long long generate_chain(unsigned long long indexInicial, int tabla);
static pthread_t newproc(void *(*tmain)(void *), void *args);
static void *child(void *v);




void 
init_table_generator(unsigned int k_length, char *cs_tag, unsigned int t_length, unsigned int ch_length, unsigned int tables){

	if(sem_init(&sem, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	if(sem_init(&sem2, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	init_reduction(k_length, d_tag);

	directory = name_directory(k_length, cs_tag, ch_length, tables);

	i_index = 0;
	collisions = 0 ;

	table_length = t_length;
	chain_length = ch_length;
	ntables = tables;

}

static const char * 
name_directory(int key_length, char *charset_tag, unsigned int chain_length, int tables){

	char kl[3];
	char cl[3];
	char nt[3];

	itoa(key_length,kl);
	itoa(chain_length,cl);
	itoa(tables,nt);

	directory[0] = '\0';
	strncat(directory,TABLE_PATH);
	strncat(directory,"rt");
	strncat(directory,"_");
	strcat(directory,kl);
	strcat(directory,"_");
	strcat(directory,domain_tag);
	strcat(directory,"_");
	strcat(directory,cl);
	strcat(directory,"_");
	strcat(directory,nt);
	strcat(directory,"/");
	printf("directorio: %s\n",directory);

	if(mkdir(directory,S_IRWXU) !=0)
		printf("Error al crear la carpeta\n");
}

static void 
name_table(char *name, int table){

	char nt[3];

	itoa(table,nt);

	name[0] = '\0';
	strcat(name,directory);
	strcat(name,nt);
}


unsigned int 
generate_table(unsigned int n_table)
{
	num_table =  n_table;
	chains = 0 ;

	char name[80];
	name_table(name,t);

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

void *
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

pthread_t
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


void 
generate_rainbow_tables()
{
	int i ;
	for(i = 0 ; i < ntables ; i++){
		generate_table(i);
	}
}


