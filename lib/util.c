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


char * 
name_rbt_package(unsigned int keylen, unsigned int *charset_types, unsigned int chainlen, unsigned int tables){

    char keylen_str[11];
    char chainlen_str[11];
    char tables_str[11];

    itoa(keylen, keylen_str);
    itoa(chainlen, chainlen_str);
    itoa(tables, tables_str);

    unsigned int pathlen = strlen(RBT_PATH_DEFAULT);
    unsigned int namelen = strlen(RBT_NAME);
    unsigned int keylen_strlen = strlen(keylen_str);
    unsigned int charset_strlen = charset_types[MIN] + charset_types[MAY] + charset_types[NUM] + charset_types[SPE];
    unsigned int chainlen_strlen = strlen(chainlen_str);
    unsigned int tables_strlen = strlen(tables_str);

    char *toReturn = malloc(pathlen + namelen + keylen_strlen + charset_strlen + chainlen_strlen + tables_strlen + 4);

    strncpy(toReturn, RBT_PATH_DEFAULT, pathlen);
    strncat(toReturn, RBT_NAME, namelen);
    strncat(toReturn, "_", 1);
    strncat(toReturn, keylen_str, keylen_strlen);
    strncat(toReturn, "_", 1);
    if(charset_types[MIN])
        strncat(toReturn, MIN_CHAR, 1);
    if(charset_types[MAY])
        strncat(toReturn, MAY_CHAR, 1);
    if(charset_types[NUM])
        strncat(toReturn, NUM_CHAR, 1);
    if(charset_types[SPE])
        strncat(toReturn, SPE_CHAR, 1);
    strncat(toReturn, "_", 1);
    strncat(toReturn, chainlen_str, chainlen_strlen);
    strncat(toReturn, "_", 1);
    strncat(toReturn, tables_str, tables_strlen);

    return toReturn;
}

char *
name_rbt_n(char *package, unsigned int table){

    char table_str[11];

    itoa(table, table_str);

    unsigned int packagelen = strlen(package);
    unsigned int table_strlen = strlen(table_str);

    char *toReturn = malloc(packagelen + table_strlen + 1);

    strncpy(toReturn, package, packagelen);
    strncat(toReturn, "/", 1);
    strncat(toReturn, table_str, table_strlen);

    return toReturn;
}