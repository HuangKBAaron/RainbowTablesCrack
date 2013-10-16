#include "sha1.h"

void sha2plain(struct SHA1Context *sha, int offset, int table, char *plain);
unsigned long long sha2index(struct SHA1Context *sha, int offset, int table);
void index2plain(unsigned long long index, char *plain);
void reduction_init(int k_length, char *tag);
