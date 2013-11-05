#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../devices/sha1.h"
#include "spclib.h"


static void reverse(char *s);


void
string2sha(char *string, SHA1Context *sha){

	char sha_text1[9];
	char sha_text2[9];
	char sha_text3[9];
	char sha_text4[9];
	char sha_text5[9];

	strncpy(&sha_text1,&string[0],8);
	sha_text1[8]= '\0';
	strncpy(&sha_text2,&string[8],8);
	sha_text2[8]= '\0';
	strncpy(&sha_text3,&string[16],8);
	sha_text3[8]= '\0';
	strncpy(&sha_text4,&string[24],8);
	sha_text4[8]= '\0';
	strncpy(&sha_text5,&string[32],8);
	sha_text5[8]= '\0';

	unsigned long number = strtoul(&sha_text1,NULL,16);
	unsigned long number2 = strtoul(&sha_text2,NULL,16);
	unsigned long number3 = strtoul(&sha_text3,NULL,16);
	unsigned long number4 = strtoul(&sha_text4,NULL,16);
	unsigned long number5 = strtoul(&sha_text5,NULL,16);

	SHA1Reset(sha);

	sha->Message_Digest[0]= number;
	sha->Message_Digest[1]= number2;
	sha->Message_Digest[2]= number3;
	sha->Message_Digest[3]= number4;
	sha->Message_Digest[4]= number5;

}

void
shacopy(SHA1Context *sha1, SHA1Context *sha2){
	
	SHA1Reset(sha2);

	sha2->Message_Digest[0]= sha1->Message_Digest[0];
	sha2->Message_Digest[1]= sha1->Message_Digest[1];
	sha2->Message_Digest[2]= sha1->Message_Digest[2];
	sha2->Message_Digest[3]= sha1->Message_Digest[3];
	sha2->Message_Digest[4]= sha1->Message_Digest[4];

}

/* shacmp adapted to our test with salts */
int 
shacmp(SHA1Context *sha1, SHA1Context *sha2){

	int i;	
	for(i = 0 ; i < 5 ; i++){
		if(sha1->Message_Digest[i] != sha2->Message_Digest[i])	
			return 1;
	}
	return 0;
}


/* itoa:  convert n to characters in s */
 void itoa(int n, char *s)
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }


/* reverse:  reverse string s in place */
static void reverse(char *s)
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }


