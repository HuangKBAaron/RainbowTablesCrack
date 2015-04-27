#include "x_rainbow_crack.h"


int main (argc, argv)
        int argc;
        char *argv[];
{
    char *ptr;

    int _vflag = 0;
    int _hflag = 0;
    int _rflag = 0;
    int _fflag = 0;
    int _Tflag = 0;
    char *_rvalue = NULL;
    char * _fvalue = NULL;
    unsigned int _Tvalue = 0;


    int opt = 0;
    /* getopt_long stores the option index here. */
    int option_index = 0;

    static struct option long_options[] =
            {
                    {"version",        no_argument,       0, 'v'},
                    {"help",           no_argument,       0, 'h'},
                    {"generate",       no_argument,       0, 'g'},
                    {"maxlen",         required_argument, 0, 'M'},
                    {"minlen",         required_argument, 0, 'm'},
                    {"keyset",         required_argument, 0, 's'},
                    {"chainlen",       required_argument, 0, 'c'},
                    {"tablelen",       required_argument, 0, 'l'},
                    {"tables",         required_argument, 0, 'n'},
                    {"rainbowtable",   required_argument, 0, 'r'},
                    {"file",           required_argument, 0, 'f'},
                    {"threads",        required_argument, 0, 't'},
                    {0, 0, 0, 0}
            };

    while((opt = getopt_long(argc, argv, ":vhgM:m:s:c:l:n:r:f:t:", long_options, &option_index)) != -1){

        switch(opt){

            case 'v':
                _vflag = 1;
                break;

            case 'h':
                _hflag = 1;
                break;

            case 'r':
                _rflag = 1;
                _rvalue = optarg;
                break;

            case 'f':
                _fflag = 1;
                _fvalue = optarg;
                break;

            case 'T':
                _Tflag = 1;
                _Tvalue = strtoul(optarg, &ptr, 10);
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

    if(_vflag){
        print_version();
        return 0;
    }

    if(_hflag){
        print_usage();
        return 0;
    }

    if(! _rflag){
        printf("--rbt is required\n");
        print_usage();
        return 1;
    }

    if(! _fflag){
        /*
        printf("--chainlen is required\n");
        print_usage();
        return 1;
        */
        _fvalue = RBT_DIGEST_FILE_DEFAULT;
    }

    if(! _Tflag){
        _Tvalue = DEFAULT_THREADS;
    }

    //printf("rflag: %d, fflag: %d, Tflag: %d\n", _rflag, _fflag, _Tflag);
    //printf("rbt: %s, file: %s, threads: %u\n", _rvalue, _fvalue, _Tvalue);

    init_break(_rvalue, _Tvalue);
    break_file(_fvalue);
}
