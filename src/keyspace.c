#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "keyspace.h"
#include "util.h"
#include "charset.h"



static unsigned long long key_space;
static unsigned long long subspaces[MAX_MKEY_LENGTH]; // array of index of sizes


static unsigned int max_key_length;





void init_keyspace(unsigned int maxlen, unsigned int charset){

    init_charset(charset);

    key_space = 0;
    unsigned int i;
    for(i = 1 ; i <= maxlen ; i++){
        key_space += pow(charset_length(), i);
        subspaces[i-1] = key_space;
    }

    max_key_length = maxlen;
}

unsigned long long get_keyspace(){
    return key_space;
}

unsigned long long get_subspace(unsigned int i){
    return subspaces[i];
}

unsigned int get_key_max_length(){
    return max_key_length;
}