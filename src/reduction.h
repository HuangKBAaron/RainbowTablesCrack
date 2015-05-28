void init_reduction(unsigned int keylen, unsigned int charset);

void sha2plain(unsigned char *sha, unsigned int offset, unsigned int table, char *plain);
void sha2plain_64(unsigned char *sha, unsigned int offset, unsigned int table, char *plain);

unsigned long long sha2index(unsigned char *sha, unsigned int offset, unsigned int table);
void index2plain(unsigned long long index, char *plain);
void index2plain_64(unsigned long long index, char *plain);

void index2plain_end_n_numbers(unsigned long long index, char *plain, unsigned int n);