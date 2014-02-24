#include <math.h>
#include <stdio.h>
#include "space.h"


void
calculate_keyspace(struct space *key_space, int dom_length, int k_length){

	key_space->ks = 0;

	int i;
	for(i = 1 ; i <= k_length ; i++){
		key_space->ks += pow(dom_length, i); 
		key_space->subspaces[i-1] = key_space->ks;
	}
	key_space->key_length = k_length;

	//printf("keyspace: %llu\n",key_space->ks);	
}






