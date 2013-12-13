#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>



void string2sha(char *str, unsigned char *sha){
	unsigned char hex[3];
	int i;
	for(i=0 ; i < 20 ; i++){
		strncpy(hex,&str[i*2],2);	
		sscanf(hex,"%x",&sha[i]);
	}
}

unsigned long long
sha2index(unsigned char *sha, int offset, int table)
{
	unsigned long long *pUll = &(sha[12]);

	return ( *pUll + offset + table) % 222222222;		
}


int SHAcmp(unsigned char*sha_1, unsigned char *sha_2){
	int i;
	for(i = 0 ; i < 20 ; i++){
		if(sha_1[i] != sha_2[i])
			return i;
	}
	return 0;
}

int main(void) {

	char r[4] = "abc";
	char *str = "a9993e364706816aba3e25717950c26c9cd0d89d";
	unsigned char sha[20];
	unsigned char sha2[20];
	unsigned char sha3[20];

	string2sha(str,sha);
	SHA1(r, strlen(r), sha2);

	strcpy(sha3,sha2);
	printf("cmp: %d\n",strcmp(r,"abc"));
	printf("cmp: %d\n",SHAcmp(sha,sha2));

	return 0;
}


/*
int main(void) {

	char *str = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
	unsigned char sha[20];

	string2sha(str,sha);

	int i;
	for(i=0 ; i < 20 ; i++){
		printf("%02x ",sha[i]);
	}
	printf("\n");

	return 0;
}*/
