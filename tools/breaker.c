#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include "../devices/sha1.h"
#include "../devices/reduction.h"
#include "../lib/spclib.h"
#include "../lib/hashTable3.h"
#include "../lib/space.h"
#include "breaker.h"


const char TABLE_RUTE[12] = "../storage/";

const char DELIMITER1[2] = "_";
const char DELIMITER2[2] = "/";

enum{ MAX_TABLES = 10};



sem_t  sem;  /* Semaforo */
sem_t  sem2;  /* Semaforo 2 */
sem_t  sem3;  /* Semaforo 3 */


static int nchilds;

static unsigned int hashes;
static unsigned int cracks;

unsigned int chain_length;
int num_tables;

FILE *fhashesp;
static Mmp_Hash tables[MAX_TABLES];




static void break_down_start(char *dir, int threads);
static void load_hashes_file(char *file);
static void load_rainbow_tables(char *dir);
static void search_sha(SHA1Context *searchedSha, unsigned long initWord, unsigned int max_ite, int table, char *r);
static int lookup(SHA1Context *searchedSha, int table);
static pthread_t newproc(void *(*tmain)(void *), void *args);
static void *child(void *v);




static void 
break_down_start(char *dir, int threads){

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

	nchilds = threads;

	hashes = 0;
	cracks = 0 ;

	int k_length;
	char tag[5];
	char str[80];
	char *pch;

	strcpy(str,dir);
  
  	pch = strtok (str, DELIMITER1);
	
  	pch = strtok (NULL, DELIMITER1);
	k_length = atoi(pch);

	pch = strtok (NULL, DELIMITER1);
	strcpy(tag,pch);

	pch = strtok (NULL, DELIMITER1);
	chain_length = atoi(pch);		// init chain_length

	pch = strtok (NULL, DELIMITER2);
	num_tables = atoi(pch);			// init num_tables

	reduction_init(k_length,tag);
}


//rt_keylenth_keydomain_chainlength_tables/1
static void 
load_rainbow_tables(char *dir){
	char loc[80];
	char table[3];

	int i;
	for(i = 0 ; i < num_tables ; i++){
		strcpy(loc,dir);
		itoa(i,table);
		strcat(loc,table);
		init_hash_table3(&tables[i], loc);	
	}
}

static void 
load_hashes_file(char *file){

	fhashesp = open(file,O_RDONLY,0777);

	if(fhashesp == NULL){
		printf("Eror al leer el archivo.\n");
	}
}

void
break_down(char *dir, char *hashes_file, int threads){
	
	break_down_start(dir, threads);

	load_rainbow_tables(dir);
	load_hashes_file(hashes_file);

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


	printf("Se ha roto %u passwords de un total de %d, lo que supone un tasa de exito del %d%\n",cracks,hashes,cracks*100/hashes);
	
	for(i = 0 ; i < num_tables ; i++){
		close_hash_table3(&tables[i]);		
	}

	close(fhashesp);		
}



static int
lookup(SHA1Context *searchedSha, int t){
	
	SHA1Context sha;
	char r[MAX_KEY_LENGTH+1];	
	char plain_result[MAX_KEY_LENGTH+1];
	unsigned long long index;
	unsigned int i_index;
	int k;

	int i;
	for(i = chain_length-1; i >= 0 ; i--){	

		shacopy(searchedSha,&sha);
		index = sha2index(&sha,i,t);

		for(k= i+1; k < chain_length ; k++){
						
			index2plain(index,r);

			SHA1Reset(&sha);
    			SHA1Input(&sha, (const unsigned char *) r, strlen(r));
			if (!SHA1Result(&sha)){
        			printf("ERROR-- could not compute message digest\n");
				break;
    			}

			index = sha2index(&sha,k,t);
		}
		
		i_index = get3(&tables[t],index);		

		if(i_index){							// case find collision
			search_sha(searchedSha,i_index,i,t,plain_result);
			if(strcmp(plain_result,"")!=0){
				printf("%s\n",plain_result);
				return 1;
			}				
		}
	}
	
	return 0;
}




static void
search_sha(SHA1Context *searchedSha, unsigned long initWord, unsigned int max_ite, int table, char *r){
	
	SHA1Context sha;	

	unsigned long long index = initWord;
	index2plain(index,r);

	int j;
	for(j = 0; j <= max_ite ; sha2plain(&sha,j,table,r),j++){

		SHA1Reset(&sha);
    		SHA1Input(&sha, (const unsigned char *) r, strlen(r));
		if (!SHA1Result(&sha)){
        		printf("ERROR-- could not compute message digest\n");
			break;
    		}
	}
	if(shacmp(searchedSha,&sha) == 0){
		return ;	
	}

	r[0]='\0';
}



static void *
child(void *v)
{
	
	int found;
	char sha_text[41];
	struct SHA1Context sha; 

	int j, i ;
	sem_wait(&sem);	// down()

	for(i = 0 ; read(fhashesp,sha_text,sizeof(sha_text)) ; i++){
		hashes++;
		sem_post(&sem);	// up()

		string2sha(sha_text,&sha);

		for(j = 0 ; j < num_tables ; j++){

			found = lookup(&sha, j);
			if(found){
				sem_wait(&sem3);	// down()
				cracks++;
				sem_post(&sem3);	// up()

				break ;	
			}
		}
		if(!found){
			//printf("clave %d no encontrada\n", i);	
		}

		sem_wait(&sem);	// down()	
	}
	sem_post(&sem);	// up()

	pthread_exit(0);
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



