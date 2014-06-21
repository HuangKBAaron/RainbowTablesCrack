#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <openssl/sha.h>
#include "../devices/reduction.h"
#include "../lib/util.h"
#include "../lib/hashTable3.h"
#include "../lib/keyspace.h"
#include "break.h"



struct Ctx {
	unsigned int chainlen;
	unsigned int tables;
	unsigned int threads;

	FILE *fp_digest;
	Mmp_Hash tables[MAX_TABLES];
};

struct Shared{
	unsigned int crack_ctr;
	unsigned int digest_ctr;		
};

struct Ctx break_ctx;
struct Shared shared;

sem_t  sem;  /* Semaforo */
sem_t  sem2;  /* Semaforo 2 */
sem_t  sem3;  /* Semaforo 3 */



static void load_hashes_file(char *file);
static void load_rainbow_tables(char *dir);
static void search_sha(unsigned char *searchedSha, unsigned long initWord, unsigned int max_ite, int table, char *r);
static int lookup(unsigned char *searchedSha, int table);
static void *child(void *v);



//rt_keylenth_keydomain_chainlength_tables/1
static void 
load_rainbow_tables(dir){

	char loc[80];
	char table[3];

	int i;
	for(i = 0 ; i < TABLES_NUMBER ; i++){
		strcpy(loc, dir);
		itoa(i,table);
		strcat(loc,table);
		init_hash_table3(&tables[i], loc);	
	}
}

static void 
init_break(char *package, unsigned int threads){

	if(sem_init(&sem, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}
	if(sem_init(&sem2, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}
	if(sem_init(&sem3, 0, 1) == -1){
		perror( "can't init the semaphore" );
		exit(EXIT_FAILURE);
	}

	unsigned int keylen = 0;
	char *charset_types = malloc(5);

	break_ctx.threads = threads;

	read_rbt_package(package, &keylen, charset_types, &break_ctx.chainlen, &break_ctx.tables);

	load_rainbow_tables(package);

	shared.digest_ctr = 0;
	shared.crack_ctr = 0;

	reduction_init(keylen, charset_types);
}

static void 
load_digest_file(char *file){

	fhashesp = open(file,O_RDONLY,0777);

	if(fhashesp == NULL){
		printf("Eror al leer el archivo.\n");
	}
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

void
break_file(char *hashes_file){
	
	load_hashes_file(hashes_file);

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


	printf("Se ha roto %u passwords de un total de %d, lo que supone un tasa de exito del %d%\n",cracks,hashes,cracks*100/hashes);
	
	for(i = 0 ; i < TABLES_NUMBER ; i++){
		close_hash_table3(&tables[i]);		
	}

	close(fhashesp);		
}

void SHAcpy(unsigned char*sha_1, unsigned char *sha_2){
	int i;
	for(i = 0 ; i < 20 ; i++){
		sha_1[i] = sha_2[i];
	}
}

static int
lookup(unsigned char *searchedSha, int t){
	
	unsigned char sha[20];
	char r[MAX_KEY_LENGTH+1];	
	char plain_result[MAX_KEY_LENGTH+1];
	unsigned long long index;
	unsigned int i_index;
	int k;

	int i;
	for(i = CHAIN_LENGTH-1 ; i >= 0 ; i--){	

		SHAcpy(sha, searchedSha);
		index = sha2index(sha,i,t);

		for(k= i+1; k < CHAIN_LENGTH ; k++){					
			index2plain(index,r);
			SHA1(r, strlen(r), sha);
			index = sha2index(sha,k,t);
		}
		
		i_index = get3(&tables[t],index);		

		if(i_index){			// case find collision
			search_sha(searchedSha,i_index,i,t,plain_result);
			if(strcmp(plain_result,"")!=0){
				sem_wait(&sem2);	// up()
				int i;
				for(i=0 ; i < 20 ; i++){
					printf("%02x",searchedSha[i]);
				}
				printf(" -> ");
				printf("%s\n",plain_result);
				sem_post(&sem2);	// up()
				return 1;
			}				
		}
	}
	
	return 0;
}


int SHAcmp(unsigned char*sha_1, unsigned char *sha_2){
	int i;
	for(i = 3 ; i < 20 ; i++){
		if(sha_1[i] != sha_2[i]){
			return i+1;
		}
	}
	return 0;
}

static void
search_sha(unsigned char *searchedSha, unsigned long initWord, unsigned int max_ite, int table, char *r){
	
	unsigned char sha[20];	

	unsigned long long index = initWord;
	index2plain(index,r);

	int j;
	for(j = 0; j < max_ite ; sha2plain(sha,j,table,r),j++){
		SHA1(r, strlen(r), sha);
	}
	SHA1(r, strlen(r), sha);
	if(SHAcmp(searchedSha,sha) == 0){
		return ;	
	}

	r[0]='\0';
}


void string2sha(char *str, unsigned char *sha){
	unsigned char hex[3];
	int i;
	for(i=0 ; i < 20 ; i++){
		strncpy(hex,&str[i*2],2);	
		sscanf(hex,"%x",&sha[i]);
	}
}



static void *
child(void *v)
{
	int found;
	char sha_text[41];
	unsigned char sha[20];

	int j, i ;
	sem_wait(&sem);	// down()

	for(i = 0 ; read(fhashesp,sha_text,sizeof(sha_text)) ; i++){
		hashes++;
		sem_post(&sem);	// up()

		string2sha(sha_text,sha);

		for(j = 0 ; j < TABLES_NUMBER ; j++){

			found = lookup(sha, j);
			if(found){
				sem_wait(&sem3);	// down()
				cracks++;
				sem_post(&sem3);	// up()

				break ;	
			}
		}
		if(!found){
			sem_wait(&sem2);	// down()
			printf("not found\n");	
			sem_post(&sem2);	// up()
		}

		sem_wait(&sem);	// down()	
	}
	sem_post(&sem);	// up()

	pthread_exit(0);
}





