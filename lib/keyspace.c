#include <math.h>
#include <stdio.h>
#include "space.h"
#include "charset.h"



unsigned long long key_space;					// keyspace
unsigned long long subspaces[MAX_KEY_LENGTH];		// array of index of sizes

int key_length;
struct charset char_set;


void set_charset(struct domain *k_domain, char *tag);




void init_keyspace(const char *charset_tag, unsigned int length){

	key_space->ks = 0;

	int i;
	for(i = 1 ; i <= k_length ; i++){
		key_space->ks += pow(dom_length, i); 
		key_space->subspaces[i-1] = key_space->ks;
	}
	key_space->key_length = k_length;

	//printf("keyspace: %llu\n",key_space->ks);


	set_charset(tag);
}


void 
set_charset(char *tag){
	char i;
	char *pch;

	k_domain->length = 0;

	pch = strchr(tag,'a');
	if(pch != NULL){
		for(i = 'a'; i <= 'z' ; i++){
			k_domain->elements[k_domain->length] = i;
			k_domain->length++;
		}
	}
	pch = strchr(tag,'A');
	if(pch != NULL){
		for(i = 'A'; i <= 'Z' ; i++){
			k_domain->elements[k_domain->length] = i;
			k_domain->length++;
		}
	}
	pch = strchr(tag,'0');
	if(pch != NULL){
		for(i = '0'; i <= '9' ; i++){
			k_domain->elements[k_domain->length]= i;
			k_domain->length++;
		}
	}
	pch = strchr(tag,'*');
	if(pch != NULL){
		k_domain->elements[k_domain->length]= '.';
		k_domain->length++;

		k_domain->elements[k_domain->length]= '_';
		k_domain->length++;
	}
}


unsigned long long get_keyspace(){
	return key_space;
}

unsigned long long get_subspace(unsigned int i){
	return subspaces[i];
}

unsigned int get_keylength(){
	return key_length;
}

struct charset *get_charset(){
	return &char_set;
}


