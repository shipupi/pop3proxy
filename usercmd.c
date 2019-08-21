#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pop3.h"
#include "usercmd.h"

const char* const sv_colon_cmds[] = { "list", "uidl", "top",0 };
const char* const sv_crlf_cmds[] = { "user", "pass", "stat", "dele", "rset", "quit", 0 };
const char* const sv_retr_cmds[] = { "retr", 0 };

// Pasa el comando a lower case
// Agarra solo la primera paralabra
// Elimina caracteres envolventes q no esten 
// en 0-z, Habria q averiguar cuales son las
// tolerancias que tienen los servidores pop3
// respecto a este tipo de caracteres
void
sanitize_cmd (char * dest,char * src) {
	ssize_t l 	= strlen(src);
	int p 		= 0;
	int i 		= 0;
	uint8_t b 	= src[i];
	for (i = 0; i < l; i++) {
		b = src[i];
		if(b >= '0' && b <= 'z') {
			break;
		}		
	}
	while((b = src[i]) >= '0' && b <= 'z') {
		dest[p] = b;
		// lower case
		if (b >= 'A' && b <= 'Z') {
			dest[p] += 'a' - 'A';
		}
		p++;
		i++;
	}
	dest[p] = 0;
}


bool 
cmd_matches (char * str, const char * const* cmds) {
	int i = 0;
	while (cmds[i] != 0) {
		if (strcmp(str,cmds[i]) == 0) {
			return true;
		}
		i++;
	}
	return false;

}

extern enum usr_command_type 
check_command(char * str) {
	char sanitized_str[MAXCMDSIZE];
	sanitize_cmd(sanitized_str, str);
	
	if (cmd_matches(sanitized_str, sv_colon_cmds)) {
		return colon_cmd;
	} else if (cmd_matches(sanitized_str, sv_crlf_cmds)) {
		return crlf_cmd;
	} else if (cmd_matches(sanitized_str, sv_retr_cmds)) {
		return retr_cmd;
	}
	return unknown_cmd;
}

extern enum pop3_state_type 
usercmd_feed(struct pop3_parser *p, uint8_t b) {
	struct usercmd_machine *mc = &p->state_machine.usercmd_mc;
	size_t cmdLen = strlen(mc->cmd);
	if (mc->cr && b == '\n') {
		// Nos fijamos que comando hubo y segun eso elegimos
		// La maquina de estados a la que seguimos
		// Defaulteamos a server_crlf por ahora
		// usercmd_close(p);
		enum usr_command_type type = check_command(mc->cmd);
		return sv_crlf;
	} else if(b == '\r'){
		mc->cr = true;
	} else {
		mc->cr = false;
	}
	mc->cmd[cmdLen] = b;
	return usercmd;
}

void 
usercmd_init(struct pop3_parser *p) {
	struct usercmd_machine user_mc 	= {
		.cr	 = false,
		.cmd = "",
	};
	p->state_name 					= usercmd;
	p->feed_method 					= usercmd_feed;
	p->state_machine.usercmd_mc 	= user_mc;
}

