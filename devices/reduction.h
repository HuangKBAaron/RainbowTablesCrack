void sha2plain(unsigned char *sha, int offset, int table, char *plain);
unsigned long long sha2index(unsigned char *sha, int offset, int table);
void index2plain(unsigned long long index, char *plain);
void index2plain_64(unsigned long long index, char *plain);
void reduction_init(int k_length, char *tag);
unsigned long long get_keyspace(void);
