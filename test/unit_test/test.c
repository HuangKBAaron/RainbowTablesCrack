#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "hashTable.h"
#include "charset.h"




void test_hash_table() {
    Mmp_Hash hash_table;

    create_hash_table(&hash_table, "table");

    for (int j = 1; j < 100; ++j) {
        put(&hash_table, j, j+5);
    }

    unsigned int v;
    for (int k = 1; k < 100; ++k) {
        v = get(&hash_table, k);

        //v should be key + 5
        printf("v%d: %u\n", k, v);
    }

    init_iterator();
    unsigned int i;
    for (i = next_value(&hash_table); (i != 0); i = next_value(&hash_table)) {
        printf("value: %u\n", i);
    }

    close_hash_table(&hash_table);
}

void test_charset() {

    init_charset(4);
    struct arrayset *chrst = get_charset();

    for (int i = 0; i < chrst->size; ++i) {
        printf("%c\n", chrst->elements[i]);
    }


    unsigned int len = charset_length();

    printf("len: %u\n", len);
}


int main (int argc, char *argv[]) {

    //test_hash_table();
    test_charset();

}