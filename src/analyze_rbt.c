#include <stdlib.h>
#include <stdio.h>

#include "util.h"



static struct Ctx {
    unsigned int maxlen;
    unsigned int charset;
    unsigned int chainlen;
    unsigned int ntables;
};


static struct Ctx analyze_ctx;




void
init_analyze_rbt(char *package) {
    char *filename;
    unsigned int filename_len;

    filename_len = strlen(generate_ctx.rbt_package) + strlen(XRAINBOW_CRACK_CONFIG) + 2;
    filename = malloc(filename_len);
    memset(filename, '\0', filename_len);

    sprintf(filename, "%s/%s", generate_ctx.rbt_package, XRAINBOW_CRACK_CONFIG);

    char *maxlen_str = NULL;
    char *charset_str = NULL;
    char *chainlen_str = NULL;
    char *ntables_str = NULL;
    maxlen_str = get_config(2, MAXLEN_CONFIG_PARAM_NAME, filename);
    charset_str = get_config(2, CHARSET_CONFIG_PARAM_NAME, filename);
    chainlen_str = get_config(15, CHAINLEN_CONFIG_PARAM_NAME, filename);
    ntables_str = get_config(2, NTABLES_CONFIG_PARAM_NAME, filename);
}