/* Entry structure for key-value storage */
typedef struct {
    unsigned long long key;
    unsigned int value;
} entry;


/* Hash table structure for secondary storage */
typedef struct {
    int fd;
    entry * fmem;
    unsigned long long size ;
} Mmp_Hash;




unsigned int get(Mmp_Hash *h, unsigned long long key);
void put(Mmp_Hash *h, unsigned long long key, unsigned int value);
void create_hash_table(Mmp_Hash *h, char *table_name);
void init_iterator(void);
unsigned int next_value(Mmp_Hash *h);
void init_hash_table(Mmp_Hash *h, char *table_name);
void close_hash_table(Mmp_Hash *h);
void values(Mmp_Hash *h);
