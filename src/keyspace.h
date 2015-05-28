#define BRUTEFORCE_MODE 1
#define END_TWO_NUMBERS_MODE 2
#define END_FOUR_NUMBERS_MODE 4


void init_keyspace(unsigned int mode, unsigned int maxlen, unsigned int charset);
unsigned long long get_keyspace(void);
unsigned long long get_subspace(unsigned int i);
unsigned int get_key_max_length(void);