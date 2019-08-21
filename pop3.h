#ifndef pop3_H_Ds3wbvgeUHWkGm7B7QLXvXKoxlA
#define pop3_H_Ds3wbvgeUHWkGm7B7QLXvXKoxlA

#include <stdint.h>
#include <stdbool.h>

#include "buffer.h"

static const uint8_t METHOD_NO_AUTHENTICATION_REQUIRED = 0x00;
static const uint8_t METHOD_NO_ACCEPTABLE_METHODS      = 0xFF;





enum pop3_state_type {
    usercmd,
    sv_crlf,
    sv_colon,
};

struct usercmd_machine {
	bool cr;
	char *cmd;
};

union pop3_machine {
	struct usercmd_machine usercmd_mc;
//	struct sv_clrf_machine *sv_clrf_mc;
//	struct sv_colon_machine *sv_colon_mc;
};

struct pop3_parser {
	enum pop3_state_type state_name;
	union pop3_machine state_machine;
	// Le paso la union que tiene todas las maquinas
	// Segun la implementacion va a bsucar la maquina
	// de la clase en la que estoy
	
	enum pop3_state_type (*feed_method)(struct pop3_parser *p, uint8_t b);
};

/** inicializa el parser */
void pop3_parser_init (struct pop3_parser *p);

/** entrega un byte al parser. retorna true si se llego al final  */
enum pop3_state_type pop3_parser_feed (struct pop3_parser *p, uint8_t b);

#endif
