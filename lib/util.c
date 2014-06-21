#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"


static void reverse(char *s);



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

void string2sha(char *str, unsigned char *sha){
    unsigned char hex[3];
    int i;
    for(i=0 ; i < 20 ; i++){
        strncpy(hex,&str[i*2],2);   
        sscanf(hex,"%x",&sha[i]);
    }
}

int SHAcmp(unsigned char*sha_1, unsigned char *sha_2){
    int i;
    for(i = 3 ; i < 20 ; i++){
        if(sha_1[i] != sha_2[i]){
            return i+1;
        }
    }
    return 0;
}

void SHAcpy(unsigned char*sha_1, unsigned char *sha_2){
    int i;
    for(i = 0 ; i < 20 ; i++){
        sha_1[i] = sha_2[i];
    }
}

char * 
name_rbt_package(unsigned int keylen, char *charset_types, unsigned int chainlen, unsigned int tables){

    char keylen_str[11];
    char chainlen_str[11];
    char tables_str[11];

    itoa(keylen, keylen_str);
    itoa(chainlen, chainlen_str);
    itoa(tables, tables_str);

    unsigned int pathlen = strlen(RBT_PATH_DEFAULT);
    unsigned int namelen = strlen(RBT_NAME);
    unsigned int keylen_strlen = strlen(keylen_str);
    unsigned int charset_strlen = strlen(charset_types);
    unsigned int chainlen_strlen = strlen(chainlen_str);
    unsigned int tables_strlen = strlen(tables_str);

    unsigned int toReturnlen = pathlen + namelen + keylen_strlen + charset_strlen + chainlen_strlen + tables_strlen + 5;

    char *toReturn = malloc(toReturnlen + 1);

    strncpy(toReturn, RBT_PATH_DEFAULT, pathlen);
    strncat(toReturn, RBT_NAME, namelen);
    strncat(toReturn, "_", 1);
    strncat(toReturn, keylen_str, keylen_strlen);
    strncat(toReturn, "_", 1);
    strncat(toReturn, charset_types, charset_strlen);
    strncat(toReturn, "_", 1);
    strncat(toReturn, chainlen_str, chainlen_strlen);
    strncat(toReturn, "_", 1);
    strncat(toReturn, tables_str, tables_strlen);
    strncat(toReturn, "/", 1);

    toReturn[toReturnlen] = '\0';

    return toReturn;
}

char *
name_rbt_n(char *package, unsigned int table){

    char table_str[11];

    itoa(table, table_str);

    unsigned int packagelen = strlen(package);
    unsigned int namelen = strlen(RBT_NAME);
    unsigned int table_strlen = strlen(table_str);

    unsigned int toReturnlen = packagelen + namelen + table_strlen + 1;

    char *toReturn = malloc(toReturnlen + 1);

    strcpy(toReturn, package);
    strncat(toReturn, RBT_NAME, namelen);
    strncat(toReturn, "_", 1);
    strncat(toReturn, table_str, table_strlen);

    toReturn[toReturnlen] = '\0';

    return toReturn;
}


void read_rbt_package(char *package, unsigned int *keylen, char *charset_types, unsigned int *chainlen, unsigned int *tables){

    char *pch;
    pch = strstr(package, RBT_NAME);

    char *str = malloc(strlen(pch));

    strcpy(str, pch);
  
    pch = strtok (str, "_");
    
    pch = strtok (NULL, "_");
    *keylen = atoi(pch);

    pch = strtok (NULL, "_");
    strcpy(charset_types, pch);

    pch = strtok (NULL, "_");
    *chainlen = atoi(pch);      

    pch = strtok (NULL, "/");
    *tables = atoi(pch);     

    free(str);
}