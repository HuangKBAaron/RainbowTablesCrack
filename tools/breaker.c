#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include "../devices/sha1.h"
#include "../devices/reduction.h"
#include "../lib/spclib.h"
#include "../lib/hashTable3.h"
#include "../lib/space.h"
#include "breaker.h"


sem_t  sem;  /* Semaforo */
sem_t  sem2;  /* Semaforo 2 */
sem_t  sem3;  /* Semaforo 3 */


static int nchilds;

unsigned int chain_length;
int num_tables;

FILE *fhashesp;

static unsigned int hashes = 0;
static unsigned int cracks = 0 ;
static Mmp_Hash tables[MAX_TABLES];

void break_down_start(char *location, int threads);
void load_hashes_file(char *file);
void load_rainbow_tables(char *dir);
static void search_word(SHA1Context *searchedSha, unsigned long initWord, unsigned int max_ite, int table, char *r);
static int find_collision(SHA1Context *searchedSha, int table);
static pthread_t newproc(void *(*tmain)(void *), void *args);
static void *child(void *v);




void 
break_down_start(char *location, int threads){

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

	int k_length;
	char tag[5];
	char str[80];
	char * pch;

	strcpy(str,location);
  
  	pch = strtok (str, DELIMITER1);
	
  	pch = strtok (NULL, DELIMITER1);
	k_length = atoi(pch);
	printf("%d\n",k_length);
	pch = strtok (NULL, DELIMITER1);
	strcpy(tag,pch);

	pch = strtok (NULL, DELIMITER1);
	chain_length = atoi(pch);

	pch = strtok (NULL, DELIMITER2);
	num_tables = atoi(pch);

	nchilds = threads;

	reduction_init(k_length,tag);
}


//rt_keylenth_keydomain_chainlength_tables/1
void 
load_rainbow_tables(char *dir){
	char str[80];
	char nt[3];

	int i;
	for(i = 0 ; i < num_tables ; i++){
		strcpy(str,dir);
		itoa(i,nt);
		strcat(str,nt);
		init_hash_table3(&tables[i], str);	
	}
}

void 
load_hashes_file(char *file){

	fhashesp = fopen(file,"r");

	if(fhashesp == NULL){
		printf("Eror al leer el archivo.\n");
	}
}

void
break_down(char *location, char *hashes_file, int threads){
	
	break_down_start(location, threads);

	load_rainbow_tables(location);
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

	fclose(fhashesp);		
}



int
find_collision(SHA1Context *searchedSha, int t){
	
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

		if(i_index){
			search_word(searchedSha,i_index,i,t,plain_result);
			if(strcmp(plain_result,"")!=0){
				printf("%s\n",plain_result);
				return 1;
			}				
		}
	}
	
	return 0;
}




void
search_word(SHA1Context *searchedSha, unsigned long initWord, unsigned int max_ite, int table, char *r){
	
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
    		}else{
			if(shacmp(searchedSha,&sha) == 0){
				//printf("iteracion: %d max_ite: %u\n",j,max_ite);
				return ;	
			}
		}
	}
	r[0]='\0';
}



void *
child(void *v)
{
	
	int found;
	char sha_text[41];
	struct SHA1Context sha; 

	int j, i ;
	sem_wait(&sem);	// down()

	for(i = 0 ; fscanf(fhashesp,"%s",sha_text) != EOF ; i++){
		hashes++;
		sem_post(&sem);	// up()

		string2sha(sha_text,&sha);

		for(j = 0 ; j < num_tables ; j++){

			found = find_collision(&sha, j);
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



