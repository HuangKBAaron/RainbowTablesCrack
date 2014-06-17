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
#include "../lib/util.h"
#include "../lib/keyspace.h"


static struct generate_ctx{
	unsigned int tablelength;
	unsigned int chainlength;
	unsigned int tables;
	unsigned int threads;

	char *rbt_package;
}

static struct shared{
	unsigned long long collision_ctr;
	unsigned int genchain_ctr;
	unsigned int index_ctr;
	unsigned int current_table;			

	Mmp_Hash hash_table;
}

sem_t  sem;  /* Semaforo */
sem_t  sem2;  /* Semaforo 2 */



static unsigned int generate_table(int n_table);
static unsigned long long generate_chain(unsigned long long indexInicial, int tabla);
static void *child(void *v);




void 
init_rbt(unsigned int keylen, unsigned int *charset_types, 
							unsigned int tablelen, unsigned int chainlen, unsigned int tables, unsigned int threads){

	if(sem_init(&sem, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	if(sem_init(&sem2, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	init_reduction(keylen, charset_types);

	generate_ctx.tablelen = tablelen;
	generate_ctx.chainlen = chainlen;
	generate_ctx.tables = tables;
	genchain_ctr.threads = threads;

	generate_ctx.rbt_package = name_rbt_package(keylen, charset_types, chainlen, tables);

	if(mkdir(generate_ctx.rbt_package, S_IRWXU) !=0){
		printf("Error al crear la carpeta\n");
	}

	shared.collision_ctr = 0;
	shared.index_ctr = 0;

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
	shared.current_table =  n_table;
	shared.genchain_ctr = 0 ;

	char *table_name = name_rbt_n(t);

	create_hash_table3(&hash_table, table_name);

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
generate_rbt()
{
	int i ;
	for(i = 0 ; i < generate_ctx.tables ; i++){
		generate_table(i);
	}
}


