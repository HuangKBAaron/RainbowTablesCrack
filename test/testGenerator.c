#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include "../devices/sha1.h"
#include "../devices/reduction.h"

// apt-get install libcurl4-openssl-dev

const unsigned int REP = 1000000 ;




void test_1(){
    unsigned int foo = 1;
    unsigned int i;
    for( i = 0 ; i < REP ; i++ ){
        foo = (foo*i) % 99 ;  
    }
}

void test_sha1(SHA1Context *h){
    unsigned int i;
    for( i = 0 ; i < REP ; i++ ){
        SHA1Reset(h);
        SHA1Input(h, (const unsigned char *) "Password", strlen("Password"));
        if (!SHA1Result(h)){
            printf("ERROR-- could not compute message digest\n");
	    break;
        }
    }
}

void test_sha1_openssl(){
    unsigned char ibuf[] = "Password";
    unsigned char obuf[20];

    unsigned int i;
    for (i = 0; i < REP; i++) {
        SHA1(ibuf, strlen(ibuf), obuf);
    }
}

void test_reduction(SHA1Context *h){
    char *r = (char *) malloc(10);
    unsigned int i, index;
    for( i = 0 ; i < REP ; i++ ){
        index = sha2index(h, i, 1);
        index2plain(index,r);
    }
}




void main (argc, argv)
int argc;
char *argv[];
{
    clock_t t_ini1, t_fin1;
    clock_t t_ini2, t_fin2;
    double secs1, secs2;

    reduction_init(7, "a0");
    SHA1Context h;

    t_ini1 = clock();     
        test_sha1(&h);
    t_fin1 = clock();

    t_ini2 = clock();
        test_sha1_openssl();
    t_fin2 = clock();

    secs1 = (double)(t_fin1 - t_ini1) / CLOCKS_PER_SEC;
    printf("TEST1: %.16g milisegundos\n", secs1 * 1000.0);

    secs2 = (double)(t_fin2 - t_ini2) / CLOCKS_PER_SEC;
    printf("TEST2: %.16g milisegundos\n", secs2 * 1000.0);
    return 0;
}
