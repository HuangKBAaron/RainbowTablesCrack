#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "generator.h"
#include "../devices/reduction.h"
#include "../devices/sha1.h"
#include "../lib/hashTable3.h"
#include "../lib/spclib.h"
#include "../lib/space.h"


unsigned int table_length;
unsigned int chain_length;

char directory[80];

static int nchilds;

static unsigned long long coll ;
static unsigned int chains ;
static unsigned int i_index;
static int num_table ;

static Mmp_Hash hash_table;


sem_t  sem;  /* Semaforo */
sem_t  sem2;  /* Semaforo 2 */

const char TABLE_PATH[9] = "storage/";


static void init_table_generator(int t);
static void generate_start(int k_length, char *d_tag, unsigned int t_length, unsigned int ch_length, int tables, int threads);
static void name_directory(int key_length, char *domain_tag, unsigned int ch_length, int tables);
static void name_table(char *name, int table);
static unsigned int generate_table(int n_table);
static unsigned long long generate_chain(unsigned long long indexInicial, int tabla);
static pthread_t newproc(void *(*tmain)(void *), void *args);
static void *child(void *v);




static void 
generate_start(int k_length, char *d_tag, unsigned int t_length, unsigned int ch_length, int tables, int threads){

	if(sem_init(&sem, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	if(sem_init(&sem2, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	table_length = t_length;
	chain_length = ch_length;

	nchilds = threads ;

	reduction_init(k_length, d_tag);
	name_directory(k_length, d_tag, ch_length, tables);

	i_index = 0;
	coll = 0 ;

}

static void 
name_directory(int key_length, char *domain_tag, unsigned int ch_length, int tables){
	char kl[3];
	char cl[3];
	char nt[3];

	itoa(key_length,kl);
	itoa(ch_length,cl);
	itoa(tables,nt);

	directory[0] = '\0';
	strcat(directory,TABLE_PATH);
	strcat(directory,"rt");
	strcat(directory,"_");
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


static void
init_table_generator(int t){
	
	num_table =  t;
	chains = 0 ;

	char name[80];
	name_table(name,t);

	create_hash_table3(&hash_table, name);
}


unsigned int 
generate_table(int n_table)
{
	init_table_generator(n_table);

	pthread_t  *childs;
	childs = malloc(nchilds * sizeof(pthread_t));

	/*
	 * crea todos los threads
	 */
	int i;
	for(i = 0; i < nchilds ; i++){
		childs[i] = newproc(child, NULL);
	}
	/*
	 * espera a que acaben todos los threads
	 */
	for(i= 0; i <  nchilds; i++)
		pthread_join(childs[i], NULL);
	free(childs);

	close_hash_table3(&hash_table);

	printf("Colisiones: %u\n",coll);

	return i_index;	
}

static unsigned long long
generate_chain(unsigned long long indexInicial, int tabla)
{
	char r[MAX_KEY_LENGTH+1];
	SHA1Context h;

	unsigned long long index;
	index = indexInicial;

	int i;
	for(i = 0; i < chain_length ; i++){

		index2plain(index,r);

		SHA1Reset(&h);
    		SHA1Input(&h, (const unsigned char *) r, strlen(r));
		if (!SHA1Result(&h)){
        		printf("ERROR-- could not compute message digest\n");
			break;
    		}
		index = sha2index(&h, i, tabla);
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
				coll++;
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
generate_tables(int key_length, char *domain_tag, unsigned int t_length, unsigned int ch_length, int tables, int threads)
{
	generate_start(key_length, domain_tag, t_length, ch_length, tables, threads);

	int i ;
	for(i = 0 ; i < tables ; i++){
		generate_table(i);
	}
}


