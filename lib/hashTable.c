#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "hashTable.h"

enum { PERMS = 0660 };

const unsigned long prime = 179424673;	
const unsigned long scale = 7;
const unsigned long shift = 13;
const unsigned long capacity = 179424673;	
const unsigned long q = 997;			// q prime and lower tha capacity



static unsigned int findKey(Mmp_Hash *h, unsigned long long key);
static unsigned int hashValue(unsigned long long key);
static unsigned int hashValue2(unsigned long long key);



/** Put a key-value pair in the map, replaceing previous one if it exists */
void put(Mmp_Hash *h, unsigned long long k, unsigned int v){

    unsigned int ind = findKey(h,k);

    h->fmem[ind].key = k ;
    h->fmem[ind].value = v ;
}

/** Returns the value associated with a key */
unsigned int get(Mmp_Hash *h, unsigned long long key){

    unsigned int ind = findKey(h,key);

    return h->fmem[ind].value ;
}

/* Find a key and return the index according to key. If it is not found, it return
a new index for the given key. DobleHash is used to solved collisions */
static unsigned int findKey(Mmp_Hash *h, unsigned long long key){

    unsigned int value;

    unsigned int i = hashValue(key);
    unsigned int j = i;
    unsigned long long k;
    unsigned int v;
    do{
        k = h->fmem[i].key ;

        if((k == key) || (k == 0)){
            break;
        }

        i = (i + hashValue2(key)) % capacity;
    }while(i != j);

    return i;
}

/** Hash function */
static unsigned int hashValue(unsigned long long key){

    return (((key*scale + shift) % prime) % capacity);
}

/** Second hash function to solve collisions */
static unsigned int hashValue2(unsigned long long key){

    return (q - (key % q));
}


void values(Mmp_Hash *h) {

}


void create_hash_table(Mmp_Hash *h, char *table_name){

    unsigned int size = capacity * sizeof(*(h->fmem));

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

    h->fmem = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, h->fd, 0);
    if(h->fmem == MAP_FAILED){
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

}

void init_hash_table(Mmp_Hash *h, char *table_name){

    unsigned int size = capacity * sizeof(*(h->fmem));

    h->fd = open(table_name, O_RDWR, PERMS);
    if(h->fd < 0){
        fprintf(stderr, "can't create the file\n");
        exit(1);
    }

    h->fmem = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, h->fd, 0);
    if(h->fmem == MAP_FAILED){
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
}

/** unmap an close the file	 */
void close_hash_table(Mmp_Hash *h){

    unsigned int size = capacity * sizeof(*(h->fmem));

    if(munmap(h->fmem, size) < 0){
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }
    close(h->fd);
}

