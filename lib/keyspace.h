enum{ MAX_KEY_LENGTH = 14};		/* Highest key length. */
enum {DOM_MAX = 100};		/* Maximum domain cardinality. */




struct charset {
	char elements[DOM_MAX];
	int length;
};


void init_keyspace(const char *charset_tag, unsigned int length);
unsigned long long get_keyspace();
unsigned long long get_subspace(unsigned int i);
unsigned int get_keylength();
struct charset *get_charset();

