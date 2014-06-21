#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "keyspace.h"
#include "util.h"



static unsigned long long key_space;					
static unsigned long long subspaces[MAX_MKEY_LENGTH];		// array of index of sizes


static unsigned int max_key_length;
static struct arrayset charset;



static void init_charset(struct arrayset *charset, char *charset_types);




static void 
init_charset(struct arrayset *charset, char *charset_types){

	char i;

	charset->size = 0;

	if(charset_types[MIN] == MIN_CHAR){
		for(i = 'a'; i <= 'z' ; i++){
			charset->elements[charset->size] = i;
			charset->size++;
		}
	}

	if(charset_types[MAY] == MAY_CHAR){
		for(i = 'A'; i <= 'Z' ; i++){
			charset->elements[charset->size] = i;
			charset->size++;
		}
	}

	if(charset_types[NUM] == NUM_CHAR){
		for(i = '0'; i <= '9' ; i++){
			charset->elements[charset->size]= i;
			charset->size++;
		}
	}

	if(charset_types[SPE] == SPE_CHAR){
		charset->elements[charset->size]= '.';
		charset->size++;

		charset->elements[charset->size]= '_';
		charset->size++;
	}
}

void init_keyspace(unsigned int maxkeylength, char *charset_types){

	init_charset(&charset, charset_types);

	key_space = 0;

	unsigned int i;
	for(i = 1 ; i <= maxkeylength ; i++){
		key_space += pow(charset.size, i); 
		subspaces[i-1] = key_space;
	}

	max_key_length = maxkeylength;	
}

unsigned long long get_keyspace(){
	return key_space;
}

unsigned long long get_subspace(unsigned int i){
	return subspaces[i];
}

unsigned int get_key_length(){
	return max_key_length;
}

struct arrayset *get_charset(){
	return &charset;
}