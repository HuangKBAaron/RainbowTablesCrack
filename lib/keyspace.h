enum{ MAX_KEY_LENGTH = 14};		/* Highest key length. */
enum {CHARSET_MAX = 100};		/* Maximum charset cardinality. */




struct arrayset {
	char elements[CHARSET_MAX];
	int size;
};


void init_keyspace(const char *charset_tag, unsigned int maxkeylength);
unsigned long long get_keyspace(void);
unsigned long long get_subspace(unsigned int i);
unsigned int get_max_key_length(void);
struct arrayset *get_charset(void);

