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
        strncpy(hex, &str[i*2], 2); 
        hex[2] = '\0';  
        sscanf(hex, "%x", &sha[i]);
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
name_rbt_package(unsigned int maxlen, unsigned int charset, unsigned int chainlen, unsigned int ntables){

    char *toReturn = NULL;

    toReturn = malloc(BUF_SIZE);
    memset(toReturn, '\0', BUF_SIZE);
    sprintf(toReturn, "%s%s_%u_%u_%u_%u/", RBT_PATH_DEFAULT, RBT_NAME, maxlen, charset, chainlen, ntables);

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


void read_rbt_package(char *package, unsigned int *maxlen, char *charset_types, unsigned int *chainlen, unsigned int *tables){

    char *pch;
    pch = strstr(package, RBT_NAME);

    char *str = malloc(strlen(pch));

    strcpy(str, pch);
  
    pch = strtok (str, "_");
    
    pch = strtok (NULL, "_");
    *maxlen = atoi(pch);

    pch = strtok (NULL, "_");
    strcpy(charset_types, pch);

    pch = strtok (NULL, "_");
    *chainlen = atoi(pch);      

    pch = strtok (NULL, "/");
    *tables = atoi(pch);     

    free(str);
}