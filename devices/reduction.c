#include <string.h>
#include <stdio.h>
#include "reduction.h"
#include "sha1.h"
#include "../lib/space.h"
#include "../lib/domain.h"



static struct space key_space;
static struct domain key_domain;	

	


static int word_length(unsigned long long index);
static unsigned long long relative_index(unsigned long long index, int size);



void 
reduction_init(int k_length, char *tag){

	set_key_domain(&key_domain, tag);
	calculate_keyspace(&key_space, key_domain.length, k_length);
}



/*reduction function: a different function for each column*/
/*transform a sha into a word*/
void
sha2plain(struct SHA1Context *sha, int offset, int table, char *plain)
{
	unsigned long long *pUl = &(sha-> Message_Digest[3]);

	index2plain( (*pUl + offset + table) % key_space.ks, plain);		
}


/*transform a sha into a index*/
unsigned long long
sha2index(struct SHA1Context *sha, int offset, int table)
{
	unsigned long long *pUl = &(sha-> Message_Digest[3]);

	return ( *pUl + offset + table) % key_space.ks;		
}


/*each word is represented by a index*/
void
index2plain(unsigned long long index, char *plain)
{
	int klength = word_length(index);

	unsigned long long ind = relative_index(index, klength);

	int j= 0;
	while(ind >= key_domain.length){
		plain[j] = key_domain.elements[ind%key_domain.length];
		ind /= key_domain.length;
	
		j++;
	}
	plain[j++] = key_domain.elements[ind%key_domain.length]; 
	
	for( ; j < klength ; j++){
		plain[j] = key_domain.elements[0];
	}		

	plain[j] = '\0';

}

static unsigned long long 
relative_index(unsigned long long index, int size){
	if (size > 1)
		return index - key_space.subspaces[size-2];
	return index;
}

static int 
word_length(unsigned long long index){
	int i;
	for( i = 0 ; i < MAX_KEY_LENGTH ; i++){
		if(index < key_space.subspaces[i])
			return i+1;
	}
	return 0;
}
