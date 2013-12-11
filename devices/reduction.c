#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>
#include "reduction.h"
#include "../lib/space.h"
#include "../lib/domain.h"


static struct space key_space;
static struct domain key_domain;	


static int reduction_length(unsigned long long index);



void 
reduction_init(int k_length, char *tag){

	set_key_domain(&key_domain, tag);
	calculate_keyspace(&key_space, key_domain.length, k_length);
}



/* reduction function: a different function for each column */
/* transform a sha into a plaintext */
void
sha2plain(unsigned char *sha, int offset, int table, char *plain)
{
	unsigned long long *pUll = &(sha[12]);

	index2plain( (*pUll + offset + table) % key_space.ks, plain);		
}


/*transform a sha into a index*/
unsigned long long
sha2index(unsigned char *sha, int offset, int table)
{
	unsigned long long *pUll = &(sha[12]);

	return ( *pUll + offset + table) % key_space.ks;		
}


/* transform a index into a plaintext */
void
index2plain(unsigned long long index, char *plain)
{
	int rlength = reduction_length(index);

	unsigned long long ind = index;

	int j;
	for( j = 0 ; j < rlength ; j++ ){
		plain[j] = key_domain.elements[ind%key_domain.length];
		ind /= key_domain.length;
	}

	plain[j] = '\0';
}

static int 
reduction_length(unsigned long long index){
	int i;
	for( i = key_space.key_length - 2 ; i >= 0 ; i--){
		if(index >= key_space.subspaces[i])
			return i+2;
	}
	return 0;
}


unsigned long long get_keyspace(){
	return key_space.ks;
}
