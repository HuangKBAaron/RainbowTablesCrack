#include <stdlib.h>
#include <stdio.h>

#include "analyze_rbt.h"

#include "util.h"
#include "reduction.h"


static struct Ctx analyze_ctx;

Mmp_Hash rbt_tables[MAX_TABLES];



void read_rbt_config(char *filename, struct Ctx *ctx) {
    char *maxlen_str = get_config(2, MAXLEN_CONFIG_PARAM_NAME, filename);
    char *charset_str = get_config(2, CHARSET_CONFIG_PARAM_NAME, filename);
    char *chainlen_str = get_config(15, CHAINLEN_CONFIG_PARAM_NAME, filename);
    char *ntables_str = get_config(2, NTABLES_CONFIG_PARAM_NAME, filename);

    unsigned int max_len = clean_int(maxlen_str);
    unsigned int char_set = clean_charset(charset_str);
    unsigned int chain_len = clean_int(chainlen_str);
    unsigned int n_tables = clean_int(ntables_str);

    ctx -> maxlen = max_len;
    ctx -> charset = char_set;
    ctx -> chainlen = chain_len;
    ctx -> ntables = n_tables;
}


void
init_analyze_rbt(char *package) {
    char *filename;
    unsigned int filename_len;

    filename_len = strlen(package) + strlen(XRAINBOW_CRACK_CONFIG) + 2;
    filename = malloc(filename_len);
    memset(filename, '\0', filename_len);

    sprintf(filename, "%s/%s", package, XRAINBOW_CRACK_CONFIG);

    read_rbt_config(filename, &analyze_ctx);

    load_rainbow_tables(package, analyze_ctx.ntables, rbt_tables);
    init_reduction(analyze_ctx.maxlen, analyze_ctx.charset);

#ifdef DEBUG

    printf("maxlen_2: %u\ncharset_2: %u\nchainlen_2: %u\nntables_2: %u\n",
           analyze_ctx.maxlen, analyze_ctx.charset, analyze_ctx.chainlen, analyze_ctx.ntables);

#endif
}

void
analyze_rbt() {
    unsigned int index;

    int i, j, k;
    for (i = 0 ; i < analyze_ctx.ntables ; i++) {

        index = get(rbt_tables[i], index);

    }


    int i;
    for(i = 0 ; i < analyze_ctx.chainlen ; i++){

    }
}