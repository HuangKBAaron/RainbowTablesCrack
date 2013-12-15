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
	for(i = 3 ; i < 20 ; i++){
		if(sha_1[i] != sha_2[i])
			return i+1;
	}
	return 0;
}

void SHAcpy(unsigned char*sha_1, unsigned char *sha_2){
	int i;
	for(i = 0 ; i < 20 ; i++){
		sha_1[i] = sha_2[i];
	}
}

int main(void) {

	char r[4] = "abc";
	char *str = "000000364706816aba3e25717850c26c9cd0d89d";
	unsigned char sha[20];
	unsigned char sha2[20];
	unsigned char sha3[20];
	unsigned long long ind1;
	unsigned long long ind2;

	string2sha(str,sha);
	SHA1(r, strlen(r), sha2);
	SHAcpy(sha3,sha);
	ind1 = sha2index(sha3,1,1);
	ind2 = sha2index(sha2,1,1);

	printf("cmp: %d\n",SHAcmp(sha3,sha2));
	printf("cmp: %d\n",ind1==ind2);

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
