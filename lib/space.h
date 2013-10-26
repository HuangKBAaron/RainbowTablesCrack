#define MAX_KEY_LENGTH 14		/* Highest key length. */



struct  space {
	unsigned long long ks;					// keyspace
	unsigned long long subspaces[MAX_KEY_LENGTH];		// array of index of sizes
	int key_length;
};



void calculate_keyspace(struct space *ks, int d_length, int k_length);
