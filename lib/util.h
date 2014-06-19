#define MAX_MKEY_LENGTH     14
#define DEFAULT_MKEY_LENGTH  8
#define MIN_MKEY_LENGTH      1

#define MAX_TABLE_LENGTH    1000000000
#define DEFAULT_TABLE_LENGTH    100000
#define MIN_TABLE_LENGTH             1

#define MAX_CHAIN_LENGTH     100000
#define DEFAULT_CHAIN_LENGTH   1000
#define MIN_CHAIN_LENGTH          1

#define MAX_TABLES    10
#define DEFAULT_TABLES 4
#define MIN_TABLES     1

#define MAX_THREADS    40
#define DEFAULT_THREADS 8
#define MIN_THREADS     1

#define RBT_PATH_DEFAULT "../storage/"
#define RBT_NAME "rbt"

#define MIN_CHAR "a"
#define MAY_CHAR "A"
#define NUM_CHAR "0"
#define SPE_CHAR "*"


enum CharsetType {
	MIN,
	MAY,
	NUM,
	SPE
}; 




void itoa(int n, char *s);
char *name_rbt_package(unsigned int keylen, unsigned int *charset_types, unsigned int chainlen, unsigned int tables);
char *name_rbt_n(char *package, unsigned int table);
