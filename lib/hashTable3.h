#define PERMS 0660




/* Entry structure for key-value storage */
typedef struct {
	unsigned long long key;
	unsigned int value;
}entry;


/* Hash table structure for secondary storage */
typedef struct {
	int fd;
	entry * fmem;
	unsigned long long size ;
}Mmp_Hash;




unsigned int get3(Mmp_Hash *h, unsigned long long key);
void put3(Mmp_Hash *h, unsigned long long key, unsigned int value);
void create_hash_table3(Mmp_Hash *h, char *table_name);
void init_hash_table3(Mmp_Hash *h, char *table_name);
void close_hash_table3(Mmp_Hash *h);
