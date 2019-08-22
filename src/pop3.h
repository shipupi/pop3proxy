#ifndef pop3_H_Ds3wbvgeUHWkGm7B7QLXvXKoxlA
#define pop3_H_Ds3wbvgeUHWkGm7B7QLXvXKoxlA

#include <stdint.h>
#include <stdbool.h>
#include "buffer.h"

#define MAXCMDSIZE 1000 // ESTE DEFINE DBEERIA ESTAR EN USERCMD.H PERO NO SE COMO PASARLO SIN ROMPER TODO

enum pop3_state_type {
    usercmd,
    servercrlf,
    servercolon,
};

struct usercmd_machine {
    bool cr;
    char cmd[MAXCMDSIZE];
};

struct servercrlf_machine {
    bool cr;
};


// Este enum me gustaria q este en servercolon.h, pero tengo problemas 
// con los iports tambien
enum servercolon_state {
    servercolon_start,
    servercolon_error, /* Este estado podria redireccionar a una maquina crlf */
    servercolon_ok,
    servercolon_end1,
    servercolon_end2,
    servercolon_end3,
    servercolon_end4,
};

struct servercolon_machine {
    enum servercolon_state state;
    bool transformations;
};

union pop3_machine {
    struct usercmd_machine      usercmd_mc;
    struct servercrlf_machine   servercrlf_mc;
    struct servercolon_machine  servercolon_mc;
};

struct pop3_parser {
    enum    pop3_state_type state_name;
    /* 
    * Le paso la union que tiene todas las maquinas
    * Segun la implementacion va a bsucar la maquina
    * de la clase en la que estoy
    */

    union   pop3_machine state_machine;
    enum    pop3_state_type (*feed_method)(struct pop3_parser *p, uint8_t b);
};

/** inicializa el parser */
void pop3_parser_init (struct pop3_parser *p);

/** entrega un byte al parser. retorna true si se llego al final  */
enum pop3_state_type pop3_parser_feed (struct pop3_parser *p, uint8_t b);

/** entrega un buffer al parser y llama a parser_feed con todos los bytes */
enum pop3_state_type pop3_consume(struct pop3_parser *p, buffer *b);

#endif
