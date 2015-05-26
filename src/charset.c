#include <stdlib.h>
#include <stdio.h>

#include "charset.h"


static struct arrayset charset_ctx;


void
init_charset(unsigned int charset) {

    char i;
    unsigned int code = charset;
    charset_ctx.size = 0;

    if(code >= SPECIALCHARS) {
        charset_ctx.elements[charset_ctx.size]= '.';
        charset_ctx.size++;

        charset_ctx.elements[charset_ctx.size]= '_';
        charset_ctx.size++;

        code -= SPECIALCHARS;
    }

    if(code >= UPPERALPHA) {
        for(i = 'A'; i <= 'Z' ; i++){
            charset_ctx.elements[charset_ctx.size] = i;
            charset_ctx.size++;
        }
        code -= UPPERALPHA;
    }

    if(code >= LOWERALPHA) {
        for(i = 'a'; i <= 'z' ; i++){
            charset_ctx.elements[charset_ctx.size] = i;
            charset_ctx.size++;
        }
        code -= LOWERALPHA;
    }

    if(code >= NUMERIC) {
        for(i = '0'; i <= '9' ; i++){
            charset_ctx.elements[charset_ctx.size]= i;
            charset_ctx.size++;
        }
        code -= NUMERIC;
    }
}

void add_elements(char *elements) {
    char *chars;
    char c;

    chars = elements;
    while((c = *chars++) != '\0'){
        charset_ctx.elements[charset_ctx.size]= c;
        charset_ctx.size++;
    }
}

struct arrayset *get_charset(){
    return &charset_ctx;
}

unsigned int charset_length() {
    return charset_ctx.size;
}