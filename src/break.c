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

#include "break.h"

#include "reduction.h"
#include "util.h"
#include "hashTable.h"




static struct Ctx {
    unsigned int chainlen;
    unsigned int ntables;
    unsigned int nthreads;

    FILE *fp_digest;
    Mmp_Hash rbt_tables[MAX_TABLES];
};

static struct Shared{
    unsigned int crack_ctr;
    unsigned int digest_ctr;
};

static struct Ctx break_ctx;
static struct Shared shared;

static sem_t  sem;
static sem_t  sem2;
static sem_t  sem3;
static sem_t  sem4;



static void load_digest_file(char *file);
static void load_rainbow_tables(char *package, unsigned int n_tables, Mmp_Hash *rbt_tables);
static void search_sha(unsigned char *searchedSha, unsigned long initWord, unsigned int max_ite, int table, char *r);
static int lookup(unsigned char *searchedSha, int table);
static void *child(void *v);



static void
load_rainbow_tables(char *package, unsigned int n_tables, Mmp_Hash *rbt_tables){

    unsigned int packagelen = strlen(package);
    unsigned int namelen = strlen(RBT_NAME);
    unsigned int ilen = 11;
    unsigned int pkg_table_namelen = packagelen + namelen + ilen + 1;

    char *i_str = malloc(ilen);
    char *pkg_table_name = malloc(pkg_table_namelen + 1);

    unsigned int i;
    for(i = 0 ; i < n_tables ; i++){

        itoa(i, i_str);

        strcpy(pkg_table_name, package);
        strcat(pkg_table_name, RBT_NAME);
        strcat(pkg_table_name, "_");
        strcat(pkg_table_name, i_str);

        init_hash_table(&rbt_tables[i], pkg_table_name);
    }

    free(i_str);
    free(pkg_table_name);
}

void
init_break(char *package, unsigned int threads){

    if((sem = sem_open("semaphore1", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror( "can't init the semaphore" );
        exit(EXIT_FAILURE);
    }
    if((sem2 = sem_open("semaphore2", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror( "can't init the semaphore" );
        exit(EXIT_FAILURE);
    }
    if((sem3 = sem_open("semaphore3", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror( "can't init the semaphore" );
        exit(EXIT_FAILURE);
    }
    if((sem4 = sem_open("semaphore4", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror( "can't init the semaphore" );
        exit(EXIT_FAILURE);
    }

    unsigned int maxlen = 0;
    unsigned int charset = 0;

    //read_rbt_package(package, maxlen, charset, &break_ctx.chainlen, &break_ctx.ntables);
    break_ctx.nthreads = threads;

    load_rainbow_tables(package, break_ctx.ntables, &break_ctx.rbt_tables);
    init_reduction(maxlen, charset);

    shared.digest_ctr = 0;
    shared.crack_ctr = 0;

    printf("\n");
    printf("loading rainbow tables...\n");
    printf("RTB package: %s\n", package);
    printf("\n");
}

static void
load_digest_file(char *file){

    break_ctx.fp_digest = open(file, O_RDONLY, 0777);

    if(break_ctx.fp_digest == NULL){
        printf("can't read file.\n");
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
break_file(char *digest_file){

    load_digest_file(digest_file);
    printf("digest file -> %s\n", digest_file);
    printf("cracking passwords...\n");

    pthread_t  *childs;
    childs = malloc(break_ctx.nthreads * sizeof(pthread_t));

    /*
     * crea todos los threads
     */
    int i;
    for(i = 0; i < break_ctx.nthreads ; i++){
        childs[i] = newproc(child, NULL);
    }
    /*
     * espera a que acaben todos los threads
     */
    for(i= 0; i <  break_ctx.nthreads; i++)
        pthread_join(childs[i], NULL);
    free(childs);

    for(i = 0 ; i < break_ctx.ntables ; i++){
        close_hash_table(&break_ctx.rbt_tables[i]);
    }

    close(break_ctx.fp_digest);

    printf("\n");
    printf("Se ha roto %u passwords de un total de %d, lo que supone un tasa de exito del %d%\n",
           shared.crack_ctr, shared.digest_ctr, shared.crack_ctr*100/shared.digest_ctr);
}



static int
lookup(unsigned char *searchedSha, int t){

    unsigned char sha[20];
    char r[MAX_MKEY_LENGTH + 1];
    char plain_result[MAX_MKEY_LENGTH + 1];
    unsigned long long index;
    unsigned int i_index;
    int k;

    int i;
    for(i = break_ctx.chainlen - 1 ; i >= 0 ; i--){

        SHAcpy(sha, searchedSha);
        index = sha2index(sha, i, t);

        for(k = i + 1 ; k < break_ctx.chainlen ; k++){

            index2plain(index, r);
            SHA1(r, strlen(r), sha);
            index = sha2index(sha, k, t);
        }

        i_index = get(&break_ctx.rbt_tables[t], index);

        if(i_index){                // case find collision

            search_sha(searchedSha, i_index, i, t, plain_result);
            if(strcmp(plain_result, "") != 0){

                sem_wait(&sem2);	// up()
                int i;
                for(i=0 ; i < 20 ; i++){
                    printf("%02x", searchedSha[i]);
                }
                printf(" -> ");
                printf("%s\n", plain_result);
                sem_post(&sem2);	// up()

                return 1;
            }
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
    for(j = 0; j < max_ite ; sha2plain(sha, j, table, r), j++){
        SHA1(r, strlen(r), sha);
    }

    SHA1(r, strlen(r), sha);
    if(SHAcmp(searchedSha, sha) == 0){
        return ;
    }

    r[0] = '\0';
}

static void *
child(void *v)
{
    int found;
    char sha_text[41];
    unsigned char sha[20];

    int j, i ;
    sem_wait(&sem);	// down()

    for(i = 0 ; read(break_ctx.fp_digest, sha_text, sizeof(sha_text)) ; i++){

        shared.digest_ctr++;
        sem_post(&sem);	// up()

        string2sha(sha_text, sha);

        for(j = 0 ; j < break_ctx.ntables ; j++){

            found = lookup(sha, j);
            if(found){
                sem_wait(&sem3);	// down()
                shared.crack_ctr++;
                sem_post(&sem3);	// up()

                break ;
            }
        }
        if(!found){
            sem_wait(&sem2);	// down()
            int i;
            for(i=0 ; i < 20 ; i++){
                printf("%02x", sha[i]);
            }
            printf(" (not found)\n");
            sem_post(&sem2);	// up()
        }

        sem_wait(&sem);	// down()	
    }
    sem_post(&sem);	// up()

    pthread_exit(0);
}
