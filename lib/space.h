
#define MAX_KEY_LENGTH 14		/* Highest key lenght. */



struct  space {
	unsigned long long ks;					//maximum index of word
	unsigned long long subspaces[MAX_KEY_LENGTH];		//array of index of sizes
};



void calculate_keyspace(struct space *ks, int d_length, int k_length);
