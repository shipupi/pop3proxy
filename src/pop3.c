/**
 * request.c -- parser del pop3 de SOCKS5
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "pop3.h"
#include "buffer.h"
#include "servercrlf.h"

char * 
stateToString(enum pop3_state_type type) {
    switch(type) {
        case usercmd:
            return "usercmd";
        case servercrlf:
            return "servercrlf";
        case servercolon:
            return "servercolon";
    }
    return "unknown";
}

extern void 
pop3_parser_init(struct pop3_parser *p) {
    // Initialize first Parser
    servercrlf_init(p);
}

/**
 * Por cada elemento del buffer llama a la funcion sobrecargada feed_method
 */
enum pop3_state_type
pop3_consume(struct pop3_parser *p, buffer *b) {
    enum pop3_state_type state;
    while(buffer_can_read(b)) {
        const uint8_t c = buffer_read(b);
        state = p->feed_method(p, c);
    }
    return state;
}

int
main(const int argc, const char **argv) {
    
    struct pop3_parser parser;
    pop3_parser_init(&parser);


    printf("Nothing to do - closing\n");
    return 1;
}