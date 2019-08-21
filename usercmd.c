#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pop3.h"
#include "usercmd.h"


extern enum pop3_state_type 
usercmd_feed(struct pop3_parser *p, uint8_t b) {
	struct usercmd_machine *mc = &p->state_machine.usercmd_mc;
	size_t cmdLen = strlen(mc->cmd);
	if (mc->cr) {
		// Nos fijamos que comando hubo y segun eso elegimos
		// La maquina de estados a la que seguimos
		// Defaulteamos a server_crlf por ahora
		return sv_crlf;
	} else if(b == '\r'){
		mc->cr = true;
	} else {
		mc->cr = false;
	}
	mc->cmd[cmdLen] = b;
	printf("Current command: %s\n", mc->cmd);
	return usercmd;
}

void 
usercmd_init(struct pop3_parser *p) {
	struct usercmd_machine user_mc 	= {
		.cr	 = false,
		.cmd = malloc(MAXCMDSIZE),
	};
	memset(user_mc.cmd, 0, MAXCMDSIZE);
	p->state_name 					= usercmd;
	p->feed_method 					= usercmd_feed;
	p->state_machine.usercmd_mc 	= user_mc;
}

