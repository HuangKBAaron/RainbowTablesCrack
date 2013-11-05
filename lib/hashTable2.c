#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "lib/hashTable2.h"

enum { PERMS = 0666 };


const unsigned long prime = 179424673;	
const unsigned long scale = 7;
const unsigned long shift = 13;
const unsigned long capacity = 179424673;	
const unsigned long q = 997;			// q prime and lower tha capacity



static unsigned int findKey(Fd_Hash *h, unsigned long long key);
static unsigned int hashValue(unsigned long long key);
static unsigned int hashValue2(unsigned long long key);



/** Put a key-value pair in the map, replaceing previous one if it exists */
void put2(Fd_Hash *h, unsigned long long key, unsigned int value){

	unsigned int ind = findKey(h,key);
	
	lseek(h->fd, 12 * ind , SEEK_SET);
	write(h->fd, &key, sizeof(key));
	write(h->fd, &value, sizeof(value));
}

/** Returns the value associated with a key */
unsigned int get2(Fd_Hash *h, unsigned long long key){

	unsigned int ind = findKey(h,key);

	unsigned int value;	
	lseek(h->fd, 12 * ind + 8, SEEK_SET);
	read(h->fd, &value, sizeof(value));
	
	return value;
}

/* Find a key and return the index according to key. If it is not found, it return
a new index for the given key. DobleHash is used to solved collisions */
unsigned int findKey(Fd_Hash *h, unsigned long long key){
	
	unsigned int value;

	unsigned int i = hashValue(key);
	unsigned int j = i;
	unsigned long long k;
	unsigned int v;
	do{
		lseek(h->fd, (12) * i , SEEK_SET);		// 12 = 8 Bytes (key) + 4 Bytes (value)
		read(h->fd, &k, sizeof(k));

		if((k == key) || (k == 0)){
			break;
		}

		i = (i + hashValue2(key)) % capacity; 
	}while(i != j);

	return i;
}

/** Hash function */
unsigned int hashValue(unsigned long long key){

	return (((key*scale + shift) % prime) % capacity);
}

/** Second hash function to solve collisions */
unsigned int hashValue2(unsigned long long key){

	return (q - (key % q));
}



void create_hash_table2(Fd_Hash *h, char *table_name, int size){
	
	h->fd = open(table_name, O_CREAT|O_RDWR|O_TRUNC, PERMS);
	if(h->fd < 0){
		fprintf(stderr, "can't create the file\n");
		exit(1);
	}

	lseek(h->fd, size - 1, SEEK_SET);
	if(write(h->fd, "\0", 1) != 1){
		fprintf(stderr, "can't write the file\n");
		exit(1);
	}
}


void init_hash_table2(Fd_Hash *h, char *table_name){

	h->fd = open(table_name, O_RDWR, PERMS);
	if(h->fd < 0){
		fprintf(stderr, "can't create the file\n");
		exit(1);
	}

}

void close_hash_table2(Fd_Hash *h){
	close(h->fd);
}

