#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "x_rainbow_crack.h"
#include "charset.h"



void print_usage() {
    printf("%s\n", X_RAINBOW_CRACK_USAGE);
}

void print_version() {
    printf("%s\n", "xrainbow_crack_1.0");
}

unsigned int clean_int(char *arg) {
    return (unsigned int)atoi(arg);
}

unsigned int clean_keyset(char *arg) {
    unsigned int val;
    val = clean_int(arg);
    if (val > 0)
        return val;

    val = 0;
    if (strchr(arg, NUMERIC_KEYSET_CODE))
        val += NUMERIC;
    if (strchr(arg, LOWERALPHA_KEYSET_CODE))
        val += LOWERALPHA;
    if (strchr(arg, UPPERALPHA_KEYSET_CODE))
        val += UPPERALPHA;
    if (strchr(arg, SPECIALCHARS_KEYSET_CODE))
        val += SPECIALCHARS;
    return val;
}

int main (argc, argv)
        int argc;
        char *argv[];
{
    char *ptr;

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
    char *tvalue = NULL;
    unsigned int maxlen = 0;
    unsigned int keyset = 0;
    unsigned int chainlen = 0;
    unsigned int tablelen = 0;
    unsigned int ntables = 0;
    unsigned int nthreads = 0;

    int opt = 0;
    /* getopt_long stores the option index here. */
    int option_index = 0;

    static struct option long_options[] =
            {
                    {"version",        no_argument,       0, 'v'},
                    {"help",           no_argument,       0, 'h'},
                    {"generate",       no_argument,       0, 'g'},
                    {"maxlen",         required_argument, 0, 'm'},
                    {"keyset",         required_argument, 0, 's'},
                    {"chainlen",       required_argument, 0, 'c'},
                    {"tablelen",       required_argument, 0, 'l'},
                    {"tables",         required_argument, 0, 'n'},
                    {"rainbowtable",   required_argument, 0, 'r'},
                    {"file",           required_argument, 0, 'f'},
                    {"threads",        required_argument, 0, 't'},
                    {0, 0, 0, 0}
            };

    while((opt = getopt_long(argc, argv, ":vhgm:s:c:l:n:r:f:t:", long_options, &option_index)) != -1){

        switch(opt){

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

    if(vflag){
        print_version();
        return 0;
    }

    if(hflag){
        print_usage();
        return 0;
    }

    if(tvalue != NULL){
        nthreads = DEFAULT_THREADS;
    } else {
        nthreads = DEFAULT_THREADS;
    }

    maxlen = clean_int(mvalue);
    keyset = clean_keyset(svalue);
    chainlen = clean_int(cvalue);
    tablelen = clean_int(lvalue);
    ntables = clean_int(nvalue);
    nthreads = clean_int(tvalue);

    if(nthreads <= 0){
        nthreads = DEFAULT_THREADS;
    }

    if( gflag  &&  maxlen > 0  &&  keyset > 0  &&  chainlen > 0  &&  tablelen > 0  &&  ntables > 0 ){
        //init_generate_rbt(_kvalue, &_charset, _cvalue, _lvalue, _tvalue, _Tvalue);
        //generate_rbt();
    } else if ( rvalue != NULL  &&  fvalue != NULL ) {
        //init_break(rvalue, nthreads);
        //break_file(fvalue);
    } else {
        print_usage();
    }

}
