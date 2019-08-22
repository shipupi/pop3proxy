#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "pop3.h"
#include "servercrlf.h"

extern enum pop3_state_type 
servercrlf_feed(struct pop3_parser *p, uint8_t b) {
	struct servercrlf_machine *mc = &p->state_machine.servercrlf_mc;
	if (mc->cr && b == '\n') {
		printf("Nuevo type: %d\n", usercmd);
	} else if (b == '\r') {
		mc->cr = true;
	} else {
		mc->cr = false;
	}
	return servercrlf;

}

// Inicializacion de la maquina
// Carga los punteros del parser a las distintas funciones
// de la maquina, e inicializa variables internas
void 
servercrlf_init(struct pop3_parser *p, bool retr) {
	struct servercrlf_machine mc 	= {
		.cr = false,
	};
	p->state_name 					= servercrlf;
	p->feed_method 					= servercrlf_feed;
	p->state_machine.servercrlf_mc 	= mc;
}
