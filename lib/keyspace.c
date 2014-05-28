#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "keyspace.h"



static unsigned long long key_space;					
static unsigned long long subspaces[MAX_KEY_LENGTH];		// array of index of sizes


static int max_key_length;
static struct arrayset charset;



static void init_charset(struct arrayset *charset, char *tag);




static void 
init_charset(struct arrayset *charset, char *tag){
	char i;
	char *pch;

	charset->size = 0;

	pch = strchr(tag,'a');
	if(pch != NULL){
		for(i = 'a'; i <= 'z' ; i++){
			charset->elements[charset->size] = i;
			charset->size++;
		}
	}
	pch = strchr(tag,'A');
	if(pch != NULL){
		for(i = 'A'; i <= 'Z' ; i++){
			charset->elements[charset->size] = i;
			charset->size++;
		}
	}
	pch = strchr(tag,'0');
	if(pch != NULL){
		for(i = '0'; i <= '9' ; i++){
			charset->elements[charset->size]= i;
			charset->size++;
		}
	}
	pch = strchr(tag,'*');
	if(pch != NULL){
		charset->elements[charset->size]= '.';
		charset->size++;

		charset->elements[charset->size]= '_';
		charset->size++;
	}
}



void init_keyspace(const char *charset_tag, unsigned int maxkeylength){

	init_charset(&charset, charset_tag);

	key_space = 0;

	int i;
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

unsigned int get_max_key_length(){
	return max_key_length;
}

struct arrayset *get_charset(){
	return &charset;
}


