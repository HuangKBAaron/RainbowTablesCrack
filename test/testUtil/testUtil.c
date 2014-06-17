#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "../../lib/util.h"




void test_util(){

    unsigned int keylen = 6;
    unsigned int charset_types[4] = {1, 0, 1, 0};
    unsigned int chainlen = 10000;
    unsigned int tables = 5;

    char *pkg_name = name_rbt_package(keylen, &charset_types, chainlen, tables);

    printf("package_name is: %s\n", pkg_name);
    printf("package_name should be: /path/to/package/name_6_a0_10000_5\n");


    char *table_name = name_rbt_n(pkg_name, 8);

    printf("table_name is: %s\n", table_name);
    printf("table_name should be: /path/to/package/name_6_a0_10000_5/8\n");
}




int main (argc, argv)
int argc;
char *argv[];
{
    test_util();

    return 0;
}
