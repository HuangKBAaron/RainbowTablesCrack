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



static struct Shared{
    unsigned int crack_ctr;
    unsigned int digest_ctr;
};

static struct Ctx break_ctx;
static struct Shared shared;

FILE *fp_digest;
Mmp_Hash rbt_tables[MAX_TABLES];

#ifdef __APPLE__
static sem_t  *sem;
static sem_t  *sem2;
static sem_t  *sem3;
static sem_t  *sem4;
#else
static sem_t  sem;
static sem_t  sem2;
static sem_t  sem3;
static sem_t  sem4;
#endif

static void load_digest_file(char *file);
static void search_sha(unsigned char *searchedSha, unsigned long initWord, unsigned int max_ite, int table, char *r);
static char *lookup(unsigned char *searchedSha, int table);
static void *child(void *v);



void
init_break(char *package, unsigned int threads){

#ifdef __APPLE__

    if((sem = sem_open("semaphore1", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror( "Can't init the semaphore" );
        exit(EXIT_FAILURE);
    }
    if((sem2 = sem_open("semaphore2", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror( "Can't init the semaphore" );
        exit(EXIT_FAILURE);
    }
    if((sem3 = sem_open("semaphore3", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror( "Can't init the semaphore" );
        exit(EXIT_FAILURE);
    }
    if((sem4 = sem_open("semaphore4", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror( "Can't init the semaphore" );
        exit(EXIT_FAILURE);
    }

#else

    if(sem_init(&sem, 0, 1) == -1){
        perror( "Can't init the semaphore" );
        exit(EXIT_FAILURE);
    }
    if(sem_init(&sem2, 0, 1) == -1){
        perror( "Can't init the semaphore" );
        exit(EXIT_FAILURE);
    }
    if(sem_init(&sem3, 0, 1) == -1){
        perror( "Can't init the semaphore" );
        exit(EXIT_FAILURE);
    }
    if(sem_init(&sem4, 0, 1) == -1){
        perror( "Can't init the semaphore" );
        exit(EXIT_FAILURE);
    }

#endif

    init_ctx_from_package(&break_ctx, package);
    break_ctx.nthreads = threads;

    load_rainbow_tables(package, break_ctx.ntables, rbt_tables);
    init_reduction(break_ctx.mode, break_ctx.maxlen, break_ctx.charset);

    shared.digest_ctr = 0;
    shared.crack_ctr = 0;

    printf("\n");
    printf("Loading rainbow tables ...\n");
    printf("RTB package: %s\n", package);
    printf("\n");
}

static void
load_digest_file(char *file){

    fp_digest = open(file, O_RDONLY, 0777);

    if(fp_digest == NULL){
        printf("Can't read digest file.\n");
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
        fprintf(stderr, "Can't create pthread\n");
        exit(EXIT_FAILURE);
    }
    return thread;
}

void
break_digest_file(char *digest_file){

    load_digest_file(digest_file);
    printf("digest file -> %s\n", digest_file);
    printf("Looking for passwords ...\n");

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
        close_hash_table(&rbt_tables[i]);
    }

    close(fp_digest);

    printf("\n");
    printf("Se ha roto %u passwords de un total de %d, lo que supone un tasa de exito del %d%\n",
           shared.crack_ctr, shared.digest_ctr, shared.crack_ctr*100/shared.digest_ctr);
}



static char *
lookup(unsigned char *searchedSha, int t){

    unsigned char sha[20];
    char r[MAX_MKEY_LENGTH + 1];
    char *plain_result;
    unsigned long long index;
    unsigned int i_index;
    int k;

    plain_result = malloc(MAX_MKEY_LENGTH + 1);
    memset(plain_result, '\0', MAX_MKEY_LENGTH + 1);

    int i;
    for(i = break_ctx.chainlen - 1 ; i >= 0 ; i--){

        SHAcpy(sha, searchedSha);
        index = sha2index(sha, i, t);

        for(k = i + 1 ; k < break_ctx.chainlen ; k++){

            index2plain(index, r);
            SHA1(r, strlen(r), sha);
            index = sha2index(sha, k, t);
        }

        i_index = get(&rbt_tables[t], index);

        if(i_index){                // case find collision

            search_sha(searchedSha, i_index, i, t, plain_result);
            if(strcmp(plain_result, "") != 0){

#ifdef __APPLE__
                sem_wait(sem2);
                int i;
                for(i=0 ; i < 20 ; i++){
                    printf("%02x", searchedSha[i]);
                }
                printf(" -> ");
                printf("%s\n", plain_result);
                sem_post(sem2);
#else
                sem_wait(&sem2);
                int i;
                for(i=0 ; i < 20 ; i++){
                    printf("%02x", searchedSha[i]);
                }
                printf(" -> ");
                printf("%s\n", plain_result);
                sem_post(&sem2);
#endif
                return plain_result;
            }
        }
    }

    return NULL;
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
    char *plain;
    char sha_text[41];
    unsigned char sha[20];

    int j, i ;
#ifdef __APPLE__
    sem_wait(sem);
#else
    sem_wait(&sem);
#endif
    for(i = 0 ; read(fp_digest, sha_text, sizeof(sha_text)) ; i++) {
        shared.digest_ctr++;
#ifdef __APPLE__
        sem_post(sem);
#else
        sem_post(&sem);
#endif

        string2sha(sha_text, sha);

        for(j = 0 ; j < break_ctx.ntables ; j++){

            plain = lookup(sha, j);
            if(plain != NULL){

#ifdef __APPLE__
                sem_wait(sem3);
                shared.crack_ctr++;
                sem_post(sem3);
#else
                sem_wait(&sem3);
                shared.crack_ctr++;
                sem_post(&sem3);
#endif

                break ;
            }
        }
        if(plain == NULL){
#ifdef __APPLE__
            sem_wait(sem2);
#else
            sem_wait(&sem2);
#endif
            int i;
            for(i=0 ; i < 20 ; i++){
                printf("%02x", sha[i]);
            }
            printf(" (not found)\n");
#ifdef __APPLE__
            sem_post(sem2);
#else
            sem_post(&sem2);
#endif
        }

#ifdef __APPLE__
        sem_wait(sem);
    }
    sem_post(sem);
#else
        sem_wait(&sem);
    }
    sem_post(&sem);
#endif

    pthread_exit(0);
}

char *
break_digest(char *digest_str)
{
    char *plain = NULL;
    unsigned char sha[20];
    int j;

    string2sha(digest_str, sha);

    for(j = 0 ; j < break_ctx.ntables ; j++){

        plain = lookup(sha, j);
        if(plain != NULL){
#ifdef __APPLE__
            sem_wait(sem3);
            shared.crack_ctr++;
            sem_post(sem3);
#else
            sem_wait(&sem3);
            shared.crack_ctr++;
            sem_post(&sem3);
#endif
            return plain;
        }
    }
    if(plain == NULL){
#ifdef __APPLE__
        sem_wait(sem2);
#else
        sem_wait(&sem2);
#endif
        int i;
        for(i=0 ; i < 20 ; i++){
            printf("%02x", sha[i]);
        }
        printf(" (not found)\n");
#ifdef __APPLE__
        sem_post(sem2);
#else
        sem_post(&sem2);
#endif
    }

    return NULL;
}