#include "../devices/sha1.h"

void string2sha(char *string, SHA1Context *sha);
void shacopy(SHA1Context *sha1,SHA1Context *sha2);
int shacmp(SHA1Context *sha1, SHA1Context *sha2);
void itoa(int n, char *s);
