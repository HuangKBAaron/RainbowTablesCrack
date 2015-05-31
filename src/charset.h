#ifndef RAINBOWTABLESCRACK_CHARSET_H
#define RAINBOWTABLESCRACK_CHARSET_H

#endif //RAINBOWTABLESCRACK_CHARSET_H

#define NUMERIC 1
#define LOWERALPHA 2
#define UPPERALPHA 4
#define SPECIALCHARS 8


#define NUMBERIC_CHARSET_LENGTH 10
#define UPPERCASE_CHARSET_LENGTH 26
#define LOWERCASE_CHARSET_LENGTH 26

#define UPPERCASE_FIRST_POINT 'A'
#define LOWERCASE_FIRST_POINT 'a'
#define NUMBERS_FIRST_POINT '0'



enum {CHARSET_MAX = 100};       /* Maximum charset cardinality. */


struct arrayset {
    char elements[CHARSET_MAX];
    int size;
};




void init_charset(unsigned int charset);
void add_elements(char *elements);
struct arrayset *get_charset(void);
unsigned int charset_length(void);