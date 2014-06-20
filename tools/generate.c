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

#include "generate.h"
#include "../devices/reduction.h"
#include "../lib/hashTable3.h"
#include "../lib/util.h"
#include "../lib/keyspace.h"


struct Ctx {
	unsigned int keylen;
	unsigned int tablelen;
	unsigned int chainlen;
	unsigned int tables;
	unsigned int threads;

	char *rbt_package;
};

struct Shared{
	unsigned long long collision_ctr;
	unsigned int genchain_ctr;
	unsigned int index_ctr;
	unsigned int current_table;			

	Mmp_Hash hash_table;
};

struct Ctx generate_ctx;
struct Shared shared;

sem_t  sem;  /* Semaforo */
sem_t  sem2;  /* Semaforo 2 */



static void generate_table(unsigned int n_table);
static unsigned long long generate_chain(unsigned int init_point, unsigned int table);
static void *child(void *v);




void 
init_rbt(unsigned int keylen, unsigned int *charset_types, 
			unsigned int chainlen, unsigned int tablelen, unsigned int tables, unsigned int threads){

	if(sem_init(&sem, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	if(sem_init(&sem2, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	init_reduction(keylen, charset_types);

	generate_ctx.keylen = keylen;
	generate_ctx.tablelen = tablelen;
	generate_ctx.chainlen = chainlen;
	generate_ctx.tables = tables;
	generate_ctx.threads = threads;
	generate_ctx.rbt_package = name_rbt_package(keylen, charset_types, chainlen, tables);

	if(mkdir(generate_ctx.rbt_package, S_IRWXU) !=0){
		printf("cant't make directory\n");
		exit(1);
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
	unsigned int init_point;
	unsigned long long end_point;

	sem_wait(&sem);	// down()
	while(shared.genchain_ctr < generate_ctx.tablelen){
		sem_post(&sem);	// up()

		sem_wait(&sem2);	// down()
			shared.index_ctr++;
			init_point = shared.index_ctr;
		sem_post(&sem2);	// up()

		end_point = generate_chain(init_point, shared.current_table);

		sem_wait(&sem);	// down()
			if(get3(&(shared.hash_table), end_point)){
				shared.collision_ctr++;
			}else{
				put3(&(shared.hash_table), end_point, init_point);
				shared.genchain_ctr++;
			}
	}
	sem_post(&sem);	// up()


	pthread_exit(0);
}

static void 
generate_table(unsigned int n_table)
{
	shared.current_table =  n_table;
	shared.genchain_ctr = 0 ;

	char *table_name = name_rbt_n(generate_ctx.rbt_package, n_table);
	printf("table name: %s\n", table_name);
	create_hash_table3(&(shared.hash_table), table_name);
	
	free(table_name);
	table_name = NULL;

	pthread_t  *childs;
	childs = malloc(generate_ctx.threads * sizeof(pthread_t));

	/*
	 * crea todos los threads
	 */
	int i;
	for(i = 0; i < generate_ctx.threads ; i++){
		childs[i] = newproc(child, NULL);
	}
	/*
	 * espera a que acaben todos los threads
	 */
	for(i= 0; i <  generate_ctx.threads; i++)
		pthread_join(childs[i], NULL);
	free(childs);

	close_hash_table3(&(shared.hash_table));

	printf("Colisiones: %llu\n", shared.collision_ctr);
}

static unsigned long long
generate_chain(unsigned int init_point, unsigned int table)
{
	char *r = malloc(generate_ctx.keylen + 1);
	unsigned char sha[20];

	unsigned long long index;
	index = init_point;

	unsigned int i;
	for(i = 0; i < generate_ctx.chainlen ; i++){
		index2plain(index, r);
        SHA1(r, strlen(r), sha);	
		index = sha2index(sha, i, table);
	}
	free(r);

	return index;
}

void 
generate_rbt()
{
	unsigned int i ;
	for(i = 0 ; i < generate_ctx.tables ; i++){
		generate_table(i);
	}
}


