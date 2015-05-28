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

#include "generate_rbt.h"

#include "reduction.h"
#include "util.h"
#include "hashTable.h"



struct Shared {
    unsigned long long collision_ctr;
    unsigned int genchain_ctr;
    unsigned int index_ctr;
    unsigned int current_table;

    Mmp_Hash hash_table;
};

struct Ctx generate_ctx;
struct Shared shared;

sem_t  *sem;
sem_t  *sem2;


static void generate_table(unsigned int n_table);
static unsigned long long generate_chain(unsigned int init_point, unsigned int table);
static void *child(void *v);




void 
init_generate_rbt(unsigned int maxlen, unsigned int charset, unsigned int chainlen, unsigned int tablelen, 
                  unsigned int ntables, unsigned int nthreads) {

    if((sem = sem_open("semaphore1", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror( "can't init the semaphore" );
        exit(EXIT_FAILURE);
    }

    if((sem2 = sem_open("semaphone2", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror( "can't init the semaphore" );
        exit(EXIT_FAILURE);
    }

    shared.collision_ctr = 0;
    shared.index_ctr = 0;

    init_reduction(1, maxlen, charset);

    generate_ctx.maxlen = maxlen;
    generate_ctx.tablelen = tablelen;
    generate_ctx.chainlen = chainlen;
    generate_ctx.ntables = ntables;
    generate_ctx.nthreads = nthreads;
    generate_ctx.rbt_package = name_rbt_package(maxlen, charset, chainlen, ntables);

    if(mkdir_recursive(generate_ctx.rbt_package, S_IRWXU) != 0) {
        printf("Cant't make Rainbow Table Package\n");
        exit(1);
    }

    char *filename;
    unsigned int filename_len = strlen(generate_ctx.rbt_package) + strlen(XRAINBOW_CRACK_CONFIG) + 2;
    filename = malloc(filename_len);
    memset(filename, '\0', filename_len);

    sprintf(filename, "%s/%s", generate_ctx.rbt_package, XRAINBOW_CRACK_CONFIG);

    FILE *fd = fopen(filename, "w+");
    fprintf(fd, "%s = %u\n", MAXLEN_CONFIG_PARAM_NAME, maxlen);
    fprintf(fd, "%s = %u\n", CHARSET_CONFIG_PARAM_NAME, charset);
    fprintf(fd, "%s = %u\n", CHAINLEN_CONFIG_PARAM_NAME, chainlen);
    fprintf(fd, "%s = %u\n", NTABLES_CONFIG_PARAM_NAME, ntables);
    fclose(fd);

#ifdef DEBUG

    printf("maxlen_1: %u\ncharset_1: %u\nchainlen_1: %u\ntablelen_1: %u\nntables_1: %u\nnthreads_1: %u\npackage_1: %s\n",
           generate_ctx.maxlen, charset, generate_ctx.chainlen, generate_ctx.tablelen, generate_ctx.ntables,
           generate_ctx.nthreads, generate_ctx.rbt_package);

#endif

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

static void *
child(void *v)
{
    unsigned int init_point;
    unsigned long long end_point;

    sem_wait(sem);
    while(shared.genchain_ctr < generate_ctx.tablelen){
        sem_post(sem);

        sem_wait(sem2);
        shared.index_ctr++;
        init_point = shared.index_ctr;
        sem_post(sem2);

        end_point = generate_chain(init_point, shared.current_table);

        sem_wait(sem);
        if(get(&(shared.hash_table), end_point)){
            shared.collision_ctr++;
        }else{
            put(&(shared.hash_table), end_point, init_point);
            shared.genchain_ctr++;
        }
    }
    sem_post(sem);

    pthread_exit(0);
}

static void
generate_table(unsigned int n_table)
{
    shared.current_table =  n_table;
    shared.genchain_ctr = 0 ;
    unsigned long long before_collision_ctr = shared.collision_ctr;

    char *table_name = name_rbt_n(generate_ctx.rbt_package, n_table);

    create_hash_table(&(shared.hash_table), table_name);

    pthread_t  *childs;
    childs = malloc(generate_ctx.nthreads * sizeof(pthread_t));

    /*
     * crea todos los threads
     */
    int i;
    for(i = 0; i < generate_ctx.nthreads ; i++){
        childs[i] = newproc(child, NULL);
    }
    /*
     * espera a que acaben todos los threads
     */
    for(i= 0; i <  generate_ctx.nthreads; i++)
        pthread_join(childs[i], NULL);
    free(childs);

    close_hash_table(&(shared.hash_table));

    printf("Table %u generated successfully | discarded chains: %llu\n", n_table, shared.collision_ctr - before_collision_ctr);

    free(table_name);
}

static unsigned long long
generate_chain(unsigned int init_point, unsigned int table)
{
    char *r = malloc(generate_ctx.maxlen + 1);
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

    printf("generating rainbow tables ...\n");

    unsigned int i ;
    for(i = 0 ; i < generate_ctx.ntables ; i++) {
        generate_table(i);
    }

    printf("Total discarded chains: %llu\n", shared.collision_ctr);
}
