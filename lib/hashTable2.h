/* Hash table structure for secondary storage */
typedef struct {
	int fd;
	unsigned long long size ;
}Fd_Hash;


unsigned int get2(Fd_Hash *h, unsigned long long key);
void put2(Fd_Hash *h, unsigned long long key, unsigned int value);
void create_hash_table2(Fd_Hash *h, char *name, int size);
void init_hash_table2(Fd_Hash *h, char *table_name);
void close_hash_table2(Fd_Hash *h);
