#define MAX_MKEY_LENGTH     14
#define DEFAULT_MKEY_LENGTH  5
#define MIN_MKEY_LENGTH      1

#define MAX_TABLE_LENGTH    1000000000
#define DEFAULT_TABLE_LENGTH     10000
#define MIN_TABLE_LENGTH             1

#define MAX_CHAIN_LENGTH     100000
#define DEFAULT_CHAIN_LENGTH    500
#define MIN_CHAIN_LENGTH          1

#define MAX_TABLES    10
#define DEFAULT_TABLES 4
#define MIN_TABLES     1

#define MAX_THREADS    40
#define DEFAULT_THREADS 8
#define MIN_THREADS     1

#define XRAINBOW_CRACK_APP_DATA "xRainbowCrack"
#define XRAINBOW_CRACK_STORE "store"
#define RBT_NAME "rbt"

#define RBT_DIGEST_FILE_DEFAULT "../digest_files/test"

#define CHARSET_DEFAULT "a"

#define MIN_CHAR 'a'
#define MAY_CHAR 'A'
#define NUM_CHAR '0'
#define SPE_CHAR 's'

#define MIN_STR "a"
#define MAY_STR "A"
#define NUM_STR "0"
#define SPE_STR "s"

#define BUF_SIZE 1024

enum CharsetType {
	MIN,
	MAY,
	NUM,
	SPE
}; 




void itoa(int n, char *s);

char *name_rbt_package(unsigned int maxlen, unsigned int charset, unsigned int chainlen, unsigned int ntables);
char *name_rbt_n(char *package, unsigned int table);

void string2sha(char *str, unsigned char *sha);
int SHAcmp(unsigned char*sha_1, unsigned char *sha_2);
void SHAcpy(unsigned char*sha_1, unsigned char *sha_2);

int mkdir_recursive(const char *dir, mode_t mode);
