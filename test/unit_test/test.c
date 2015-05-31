#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "hashTable.h"
#include "charset.h"
#include "break.h"
#include "reduction.h"




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

void test_charset1() {

    init_charset(4);
    struct arrayset *chrst = get_charset();

    for (int i = 0; i < chrst->size; ++i) {
        printf("%c\n", chrst->elements[i]);
    }


    unsigned int len = charset_length();

    printf("len: %u\n", len);
}

void test_charset2() {

    init_charset(0);
    add_elements("abc");
    struct arrayset *chrst = get_charset();

    for (int i = 0; i < chrst->size; ++i) {
        printf("%c\n", chrst->elements[i]);
    }


    unsigned int len = charset_length();

    printf("len: %u\n", len);
}

void test_break() {

    // digest = sha1("290670")
    char *digest = "32672468489c2d8722573e86a7bb90c3bf4e1568";

    init_break("/Users/ivanprjcts/xRainbowCrack/rbt_test/", 8);
    char *plain = break_digest(digest);

    if (plain != NULL)
        printf("plain: %s\n", plain);
    else
        printf("plain: not found\n");
}


void test_reduction() {
    init_reduction(1, 5, 2);

    char plain[15];
    index2plain(543, plain);

    printf("plain: %s\n", plain);
}

int main (int argc, char *argv[]) {

    //test_hash_table();
    //test_charset1();
    //test_charset2();
    //test_break();
    test_reduction();

}