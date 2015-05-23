struct Ctx {
    unsigned int maxlen;
    unsigned int charset;
    unsigned int chainlen;
    unsigned int ntables;
};


void read_rbt_config(char *filename, struct Ctx *ctx);
void init_analyze_rbt(char *package);
