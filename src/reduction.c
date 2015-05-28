#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/sha.h>

#include "reduction.h"

#include "keyspace.h"
#include "charset.h"


typedef void (*functiontype)(unsigned long long, char *);

static functiontype index2plain_func;
static unsigned int end_n_subcharset_n_parameter = 0;
static unsigned int end_n_subcharset_size_parameter = 0;


static unsigned int reduction_length(unsigned long long index);
static void index2plain_bruteforce(unsigned long long index, char *plain);
static void index2plain_end_n_numbers(unsigned long long index, char *plain);



void 
init_reduction(unsigned int mode, unsigned int maxlen, unsigned int charset) {
    init_keyspace(mode, maxlen, charset);

    switch (mode) {
        case BRUTEFORCE_MODE:
            index2plain_func = &index2plain_bruteforce;
            break;
        case END_TWO_NUMBERS_MODE:
            index2plain_func = &index2plain_end_n_numbers;
            end_n_subcharset_n_parameter = 2;
            end_n_subcharset_size_parameter = 10;
            break;
        case END_FOUR_NUMBERS_MODE:
            index2plain_func = &index2plain_end_n_numbers;
            end_n_subcharset_n_parameter = 4;
            end_n_subcharset_size_parameter = 10;
            break;
    }
}



/* transform a index into a plaintext */
void
index2plain(unsigned long long index, char *plain)
{
    index2plain_func(index, plain);
}



/* reduction function: a different function for each column */
/* transform a sha into a plaintext */
void
sha2plain(unsigned char *sha, unsigned int offset, unsigned int table, char *plain)
{
    unsigned long long *pUll = &(sha[12]);

    index2plain( (*pUll + offset + table) % get_keyspace(), plain);
}

/* transform a sha into a plaintext. |Charset| must be equal to 64 */
void
sha2plain_64(unsigned char *sha, unsigned int offset, unsigned int table, char *plain)
{
    unsigned long long *pUll = &(sha[12]);

    index2plain_64( (*pUll + offset + table) % get_keyspace(), plain);
}


/*transform a sha into a index*/
unsigned long long
sha2index(unsigned char *sha, unsigned int offset, unsigned int table)
{
    unsigned long long *pUll = &(sha[12]);

    return ( *pUll + offset + table) % get_keyspace();
}

/* transform a index into a plaintext */
void
index2plain_bruteforce(unsigned long long index, char *plain)
{
    unsigned int rlength = reduction_length(index);

    unsigned long long ind = index;

    unsigned int j;
    for( j = 0 ; j < rlength ; j++ ){
        plain[j] = get_charset()->elements[ind%get_charset()->size];
        ind /= get_charset()->size;
    }

    plain[j] = '\0';
}

/* transform a index into a plaintext. |Charset| must be equal to 64 */
void
index2plain_64(unsigned long long index, char *plain)
{
    unsigned int rlength = reduction_length(index);

    unsigned long long ind = index;

    unsigned int j, k;
    for( j = 0 ; j < rlength ; j++ ){
        k = ind & 0x3F;

        plain[j] = get_charset()->elements[k];
        ind = ind >> 6;
    }

    plain[j] = '\0';
}


/* Returns the length of the word represented by the index */
static unsigned int 
reduction_length(unsigned long long index){

    int i;
    for(i = get_key_max_length() - 2 ; i >= 0 ; i--){

        if(index >= get_subspace(i))
            return i + 2;
    }
    return 1;
}


/* transform a index into a plaintext */
void
index2plain_end_n_numbers(unsigned long long index, char *plain)
{
    unsigned int rlength = reduction_length(index);

    unsigned long long ind = index;

    unsigned int j;
    for( j = 0 ; j < rlength; j++ ){
        plain[j] = get_charset()->elements[ind%get_charset()->size];
        ind /= get_charset()->size;
    }

    for(int k = 0 ; k < end_n_subcharset_n_parameter ; j++, k++) {
        plain[j] = (char) ('0' + ind % end_n_subcharset_size_parameter);
        ind /= end_n_subcharset_size_parameter;
    }

    plain[j] = '\0';
}