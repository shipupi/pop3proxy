/**
 * request.c -- parser del pop3 de SOCKS5
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "pop3.h"
#include "usercmd.h"

extern void 
pop3_parser_init(struct pop3_parser *p) {
    // Initialize first Parser
    usercmd_init(p);
}

int
main(const int argc, const char **argv) {
    
    struct pop3_parser *parser = malloc(sizeof(struct pop3_parser));
    pop3_parser_init(parser);
    enum pop3_state_type newState;
    newState = parser->feed_method(&parser->state_machine, 'l');
    newState = parser->feed_method(&parser->state_machine, 'i');
    newState = parser->feed_method(&parser->state_machine, 's');
    newState = parser->feed_method(&parser->state_machine, 't');
    newState = parser->feed_method(&parser->state_machine, '\r');
    newState = parser->feed_method(&parser->state_machine, '\n');
    if (parser->state_name != newState) {
    	printf("Distinto\n");
    } else {
    	printf("mismo\n");
    }

    return 1;
}