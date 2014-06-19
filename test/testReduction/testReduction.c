#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "../../devices/reduction.h"

// apt-get install libcurl4-openssl-dev

const unsigned int REP = 100 ;





void test_reduction(){
    char *r = (char *) malloc(10);
    unsigned long long KS = get_keyspace(); 

    unsigned int i, j;
    for( i = 0 ; i < REP ; i++ ){
        for( j = 0 ; j < KS ; j++ ){
            index2plain(j, r);
            //printf("%llu %s\n",j, r);
        }
    }
}

void test_reduction_opt(){
    char *r = (char *) malloc(10);
    unsigned long long KS = get_keyspace(); 

    unsigned int i, j;
    for( i = 0 ; i < REP ; i++ ){
        for( j = 0 ; j < KS ; j++ ){
            index2plain_64(j, r);
            //printf("%llu %s\n",j, r);
        }
    }
}


void test_index2plain(){

    unsigned int charset_test[4] = {1, 1, 1, 1};
    unsigned int keylen = 6;

    init_reduction(keylen, charset_test);

    char *r = malloc(10);  
    unsigned int i;
    for(i=0 ; i < 200 ; i++){
        index2plain(i, r);
        printf("%u -> %s\n", i, r);
    }
    
}



void main (argc, argv)
int argc;
char *argv[];
{
    /*
    clock_t t_ini1, t_fin1;
    clock_t t_ini2, t_fin2;
    double secs1, secs2;

    reduction_init(4, "aA0*");
    //test_reduction_opt();
    //test_reduction();


    t_ini1 = clock();     
        test_reduction();
    t_fin1 = clock();

    t_ini2 = clock();
        test_reduction_opt();
    t_fin2 = clock();

    secs1 = (double)(t_fin1 - t_ini1) / CLOCKS_PER_SEC;
    printf("TEST1: %.16g milisegundos\n", secs1 * 1000.0);

    secs2 = (double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC;
    printf("TEST2: %.16g milisegundos\n", secs2 * 1000.0);
    */

    test_index2plain();

    return 0;
}
