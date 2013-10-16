#include <math.h>
#include <stdio.h>
#include "space.h"




void
calculate_keyspace(struct space *key_space, int d_length, int k_length){

	key_space->ks = 0;

	int i;
	for(i = 1 ; i <= k_length ; i++){
		key_space->ks += pow(d_length, i); 
		key_space->subspaces[i-1] = key_space->ks;
	}
/**
	if(k_length == 4)
		key_space->ks = 475243;		//bigger prime lower than 475254 (mindexword for long 4)
	if(k_length == 5)
		key_space->ks = 12356551;	//bigger prime lower than 12356630 (mindexword for long 5)
	if(k_length == 6)
		key_space->ks = 321272383;	//bigger prime lower than 321272406 (mindexword for long 6)
	if(k_length == 7)
		key_space->ks = 3628273133;	//bigger prime lower than 4058115286 (mindexword for long 7)

**/
printf("%u\n",key_space->ks);	
}






