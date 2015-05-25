#include <stdlib.h>
#include <stdio.h>
#include <openssl/sha.h>

#include "analyze_rbt.h"

#include "hashTable.h"
#include "util.h"
#include "reduction.h"


static struct Ctx analyze_ctx;

Mmp_Hash rbt_tables[MAX_TABLES];



void
init_analyze_rbt(char *package) {

    init_ctx_from_package(&analyze_ctx, package);
    load_rainbow_tables(analyze_ctx.rbt_package, analyze_ctx.ntables, rbt_tables);
    init_reduction(analyze_ctx.maxlen, analyze_ctx.charset);

#ifdef DEBUG

    printf("maxlen_2: %u\ncharset_2: %u\nchainlen_2: %u\nntables_2: %u\n",
           analyze_ctx.maxlen, analyze_ctx.charset, analyze_ctx.chainlen, analyze_ctx.ntables);

#endif
}

void
analyze_rbt(unsigned int columns) {
    unsigned char sha[20];
    char r[MAX_MKEY_LENGTH+1];
    unsigned int index;

    int i, j;
    for (i = 0 ; i < analyze_ctx.ntables ; i++) {
        init_iterator();
        while ((index = next_value(&rbt_tables[i])) != 0) {
            index2plain(index, r);
            for (j = 0 ; j < analyze_ctx.chainlen ; j++) {
                printf("%s ", r);
                if (((j+1) % columns) == 0)
                    printf("\n");

                SHA1(r, strlen(r), sha);
                sha2plain(sha, j, i, r);
            }
        }
    }
    printf("\n");
}