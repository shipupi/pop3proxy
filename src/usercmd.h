#ifndef USERCMD_H
#define USERCMD_H

void
usercmd_init(struct pop3_parser *p);

enum usr_command_type {
	colon_cmd,
	crlf_cmd,
	retr_cmd,
	unknown_cmd,
};

#endif
