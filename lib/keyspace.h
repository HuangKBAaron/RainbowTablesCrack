
enum {CHARSET_MAX = 100};		/* Maximum charset cardinality. */


struct arrayset {
	char elements[CHARSET_MAX];
	int size;
};



void init_keyspace(unsigned int keylen, unsigned int *charset_types);
unsigned long long get_keyspace(void);
unsigned long long get_subspace(unsigned int i);
unsigned int get_key_lenth(void);
struct arrayset *get_charset(void);