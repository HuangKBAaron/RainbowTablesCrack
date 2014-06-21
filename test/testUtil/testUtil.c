#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "../../lib/util.h"




void test_util(){

    unsigned int keylen = 6;
    char charset_types[5] = "a0";
    unsigned int chainlen = 10000;
    unsigned int tables = 5;

    char *pkg_name = name_rbt_package(keylen, &charset_types, chainlen, tables);

    printf("\n");
    printf("package_name is: %s\n", pkg_name);
    printf("package_name should be: /path/to/package/name_6_a0_10000_5\n");
    printf("\n");


    char *table_name = name_rbt_n(pkg_name, 8);

    printf("table_name is: %s\n", table_name);
    printf("table_name should be: /path/to/package/name_6_a0_10000_5/rbt_8\n");
    printf("\n");


    unsigned int keylen_res = 0;
    char *charset_types_res = malloc(5);
    unsigned int chainlen_res = 0;
    unsigned int tables_res = 0;

    read_rbt_package(pkg_name, &keylen_res, charset_types_res, &chainlen_res, &tables_res);

    printf("keylen: %u, charset_types_res: %s, chainlen_res: %u, tables_res: %u\n", 
                                            keylen_res, charset_types_res, chainlen_res, tables_res);
    printf("\n");

}




int main (argc, argv)
int argc;
char *argv[];
{
    test_util();

    return 0;
}
