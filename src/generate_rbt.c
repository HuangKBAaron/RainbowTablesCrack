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


struct Ctx {
    unsigned int maxlen;
    unsigned int tablelen;
    unsigned int chainlen;
    unsigned int ntables;
    unsigned int nthreads;

    char *rbt_package;
};

struct Shared {
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




void 
init_generate_rbt(unsigned int maxlen, unsigned int charset, unsigned int chainlen, unsigned int tablelen, 
                  unsigned int ntables, unsigned int nthreads) {

    if(sem_init(&sem, 0, 1) == -1) {
        perror( "can't init the semaphore" );
        exit(EXIT_FAILURE);
    }

    if(sem_init(&sem2, 0, 1) == -1) {
        perror( "can't init the semaphore" );
        exit(EXIT_FAILURE);
    }

    init_reduction(maxlen, charset);

    generate_ctx.maxlen = maxlen;
    generate_ctx.tablelen = tablelen;
    generate_ctx.chainlen = chainlen;
    generate_ctx.ntables = ntables;
    generate_ctx.nthreads = nthreads;
    generate_ctx.rbt_package = name_rbt_package(maxlen, charset, chainlen, ntables);

    if(mkdir(generate_ctx.rbt_package, S_IRWXU) !=0){
        printf("Cant't make directory\n");
        exit(1);
    }

    shared.collision_ctr = 0;
    shared.index_ctr = 0;

    printf("maxlen: %u\ncharset: %u\nchainlen: %u\ntablelen: %u\nntables: %u\nthreads: %u\npackage: %s\n",
           generate_ctx.maxlen, charset, generate_ctx.chainlen, generate_ctx.tablelen, generate_ctx.ntables,
           generate_ctx.nthreads, generate_ctx.rbt_package);
}

