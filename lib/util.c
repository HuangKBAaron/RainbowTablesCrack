#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>

#include "util.h"

#include "x_rainbow_crack.h"
#include "charset.h"



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

int mkdir_recursive(const char *dir, mode_t mode) {
    char tmp[1024];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if(tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
        if(*p == '/') {
            *p = 0;
            mkdir(tmp, mode);
            *p = '/';
        }
    return mkdir(tmp, mode);
}

char * 
name_rbt_package(unsigned int maxlen, unsigned int charset, unsigned int mode, unsigned int chainlen, unsigned int ntables) {

    char *toReturn = NULL;
    struct passwd *pw;
    const char *homedir;
    time_t seconds;

    toReturn = malloc(BUF_SIZE);
    memset(toReturn, '\0', BUF_SIZE);

    pw = getpwuid(getuid());
    homedir = pw->pw_dir;

    seconds = time (NULL);

#ifdef DEBUG

    sprintf(toReturn, "%s/%s/%s_%s/", homedir, XRAINBOW_CRACK_APP_DATA, RBT_NAME, "test");

#else

    sprintf(toReturn, "%s/%s/%s_%u_%u_%u_%u_%u_%ld/", homedir, XRAINBOW_CRACK_APP_DATA, RBT_NAME, maxlen, charset, mode,
            chainlen, ntables, seconds);

#endif

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

const char *get_config(int max_size, const char* pParameter, const char* pConfig) {

    char *toReturn = NULL;
    char buffer[BUF_SIZE], *ptr;

    FILE *fp = fopen(pConfig,"r");
    if (fp == NULL) {
        return NULL;
    }

    while(fgets(buffer, BUFSIZ, fp) != NULL) {
        ptr = buffer;
        while((*ptr == ' ') || (*ptr == '\t'))
            ptr++;
        if ((*ptr == '\n') || (*ptr == '#')) /* Move to the next line */
            continue;
        if (strncmp(ptr, pParameter, strlen(pParameter)) == 0) {
            ptr += strlen(pParameter);
            while((*ptr == ' ') || (*ptr == '\t'))
                ptr++;
            if (*ptr != '=') { /* Move to the next line */
                continue;
            }
            ptr++;
            while ((*ptr == ' ') || (*ptr == '\t')) {
                ptr++;
            }
            if (*ptr != '\n') {
                toReturn = malloc(max_size + 1);
                memset(toReturn, 0, max_size + 1);
                strncpy(toReturn, (char *)ptr, max_size);
                if (toReturn[strlen(toReturn)-1] == '\n') {
                    toReturn[strlen(toReturn)-1] = '\0';
                }
                break;
            }
        }
    }

    fclose(fp);
    return toReturn;
}

unsigned int clean_int(char *arg) {
    if (arg != NULL) {
        return (unsigned int)atoi(arg);
    }
    return 0;
}

unsigned int clean_charset(char *arg) {
    if (arg == NULL)
        return 0;

    unsigned int val;
    val = clean_int(arg);
    if (val > 0)
        return val;

    val = 0;
    if (strchr(arg, NUMERIC_CHARSET_CODE))
        val += NUMERIC;
    if (strchr(arg, LOWERALPHA_CHARSET_CODE))
        val += LOWERALPHA;
    if (strchr(arg, UPPERALPHA_CHARSET_CODE))
        val += UPPERALPHA;
    if (strchr(arg, SPECIAL_CHARSET_CODE))
        val += SPECIALCHARS;
    return val;
}

void
init_ctx_from_package(struct Ctx *ctx, char *package) {
    char *filename = NULL;
    filename = get_feat_from_package(package);

    char *maxlen_str = get_config(2, MAXLEN_CONFIG_PARAM_NAME, filename);
    char *charset_str = get_config(2, CHARSET_CONFIG_PARAM_NAME, filename);
    char *mode_str = get_config(2, MODE_CONFIG_PARAM_NAME, filename);
    char *chainlen_str = get_config(15, CHAINLEN_CONFIG_PARAM_NAME, filename);
    char *ntables_str = get_config(2, NTABLES_CONFIG_PARAM_NAME, filename);

    unsigned int max_len = clean_int(maxlen_str);
    unsigned int char_set = clean_charset(charset_str);
    unsigned int mode = clean_charset(mode_str);
    unsigned int chain_len = clean_int(chainlen_str);
    unsigned int n_tables = clean_int(ntables_str);

    ctx -> maxlen = max_len;
    ctx -> charset = char_set;
    ctx -> mode = mode;
    ctx -> chainlen = chain_len;
    ctx -> ntables = n_tables;
    ctx -> rbt_package = package;
}

char *
get_feat_from_package(char *package) {

    char *filename;
    unsigned int filename_len;

    filename_len = strlen(package) + strlen(XRAINBOW_CRACK_CONFIG) + 2;
    filename = malloc(filename_len);
    memset(filename, '\0', filename_len);

    sprintf(filename, "%s/%s", package, XRAINBOW_CRACK_CONFIG);

    return filename;
}