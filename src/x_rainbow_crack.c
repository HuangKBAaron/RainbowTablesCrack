#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "x_rainbow_crack.h"

#include "analyze_rbt.h"
#include "break.h"
#include "generate_rbt.h"
#include "charset.h"
#include "util.h"



void print_usage() {
    printf("%s\n", X_RAINBOW_CRACK_USAGE);
}

void print_version() {
    printf("%s\n", "xrainbow_crack_1.0");
}


int main (int argc, char *argv[]) {
    int vflag = 0;
    int hflag = 0;
    int gflag = 0;
    char *mvalue = NULL;
    char *svalue = NULL;
    char *cvalue = NULL;
    char *lvalue = NULL;
    char *nvalue = NULL;
    char *rvalue = NULL;
    char *fvalue = NULL;
    char *avalue = NULL;
    char *tvalue = NULL;
    unsigned int maxlen = 0;
    unsigned int charset = 0;
    unsigned int chainlen = 0;
    unsigned int tablelen = 0;
    unsigned int ntables = 0;
    unsigned int nthreads = 0;

    int opt = 0;
    /* getopt_long stores the option index here. */
    int option_index = 0;

    static struct option long_options[] =
            {
                    {"version",      no_argument,       0, 'v'},
                    {"help",         no_argument,       0, 'h'},
                    {"generate",     no_argument,       0, 'g'},
                    {"maxlen",       required_argument, 0, 'm'},
                    {"charset",      required_argument, 0, 's'},
                    {"chainlen",     required_argument, 0, 'c'},
                    {"tablelen",     required_argument, 0, 'l'},
                    {"tables",       required_argument, 0, 'n'},
                    {"rainbowtable", required_argument, 0, 'r'},
                    {"file",         required_argument, 0, 'f'},
                    {"analyze",      required_argument, 0, 'a'},
                    {"threads",      required_argument, 0, 't'},
                    {0,              0,                 0, 0}
            };

    while ((opt = getopt_long(argc, argv, ":vhgm:s:c:l:n:r:f:t:a:", long_options, &option_index)) != -1) {

        switch (opt) {

            case 'v':
                vflag = 1;
                break;

            case 'h':
                hflag = 1;
                break;

            case 'g':
                gflag = 1;
                break;

            case 'm':
                mvalue = optarg;
                break;

            case 's':
                svalue = optarg;
                break;

            case 'c':
                cvalue = optarg;
                break;

            case 'l':
                lvalue = optarg;
                break;

            case 'n':
                nvalue = optarg;
                break;

            case 'r':
                rvalue = optarg;
                break;

            case 'f':
                fvalue = optarg;
                break;

            case 'a':
                avalue = optarg;
                break;

            case 't':
                tvalue = optarg;
                break;

            case ':':   /* missing option argument */
                fprintf(stderr, "%s: option `-%c' requires an argument\n", argv[0], optopt);
                break;

            case '?':

            default:    /* invalid option */
                fprintf(stderr, "%s: option `-%c' is invalid: ignored\n", argv[0], optopt);
                print_usage();
                return 1;
        }
    }

    if (argc < 2) {
        print_usage();
        return 1;
    }

    if (vflag) {
        print_version();
        return 0;
    }

    if (hflag) {
        print_usage();
        return 0;
    }

    if (tvalue != NULL) {
        nthreads = DEFAULT_THREADS;
    } else {
        nthreads = DEFAULT_THREADS;
    }

    maxlen = clean_int(mvalue);
    charset = clean_charset(svalue);
    chainlen = clean_int(cvalue);
    tablelen = clean_int(lvalue);
    ntables = clean_int(nvalue);
    nthreads = clean_int(tvalue);

    if (nthreads <= 0) {
        nthreads = DEFAULT_THREADS;
    }

    if (gflag && maxlen > 0 && charset > 0 && chainlen > 0 && tablelen > 0 && ntables > 0) {

#ifdef DEBUG

        printf("gflag_0: %d\nmaxlen_0: %u\ncharset_0: %u\nchainlen_0: %u\ntablelen_0: %u\n"
        "ntables_0: %u\n", gflag, maxlen, charset, chainlen, tablelen, ntables);

#endif

        init_generate_rbt(maxlen, charset, chainlen, tablelen, ntables, nthreads);
        generate_rbt();
    } else if (rvalue != NULL && fvalue != NULL) {

#ifdef DEBUG

        printf("rvalue_3: %s\nfvalue_3: %s\n", rvalue, fvalue);

#endif

        init_break(rvalue, nthreads);
        //break_file(fvalue);
    } else if (avalue != NULL) {

#ifdef DEBUG

    printf("avalue_2: %s\n", avalue);

#endif

        init_analyze_rbt(avalue);
        analyze_rbt(4);

    } else {

        print_usage();

    }

}
