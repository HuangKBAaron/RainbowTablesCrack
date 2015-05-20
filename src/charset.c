#include <stdlib.h>
#include <stdio.h>

#include "charset.h"


static struct arrayset charset_ctx;


void
init_charset(unsigned int charset) {

    char i;
    unsigned int code = charset;
    charset_ctx.size = 0;

    if(charset >= SPECIALCHARS) {
        charset_ctx.elements[charset_ctx.size]= '.';
        charset_ctx.size++;

        charset_ctx.elements[charset_ctx.size]= '_';
        charset_ctx.size++;

        code -= SPECIALCHARS;
    }

    if(charset >= NUMERIC) {
        for(i = '0'; i <= '9' ; i++){
            charset_ctx.elements[charset_ctx.size]= i;
            charset_ctx.size++;
        }
        code -= NUMERIC;
    }

    if(charset >= UPPERALPHA) {
        for(i = 'A'; i <= 'Z' ; i++){
            charset_ctx.elements[charset_ctx.size] = i;
            charset_ctx.size++;
        }
        code -= UPPERALPHA;
    }

    if(charset >= LOWERALPHA) {
        for(i = 'a'; i <= 'z' ; i++){
            charset_ctx.elements[charset_ctx.size] = i;
            charset_ctx.size++;
        }
        code -= LOWERALPHA;
    }
}


struct arrayset *get_charset(){
    return &charset_ctx;
}

unsigned int charset_length() {
    return charset_ctx.size;
}