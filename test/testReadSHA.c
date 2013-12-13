#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

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



int main(void) {

	char *str = "da39a3ee5e5b470d325599ef95601898af880709";
	unsigned char sha[20];

	string2sha(str,sha);

	printf("%llu\n",sha2index(sha,1,2));

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
