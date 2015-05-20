#ifndef RAINBOWTABLESCRACK_CHARSET_H
#define RAINBOWTABLESCRACK_CHARSET_H

#endif //RAINBOWTABLESCRACK_CHARSET_H

#define NUMERIC 1
#define LOWERALPHA 2
#define UPPERALPHA 4
#define SPECIALCHARS 8



enum {CHARSET_MAX = 100};       /* Maximum charset cardinality. */


struct arrayset {
    char elements[CHARSET_MAX];
    int size;
};




void init_charset(unsigned int charset);
struct arrayset *get_charset(void);
unsigned int charset_length(void);