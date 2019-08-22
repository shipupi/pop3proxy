    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "pop3.h"
    #include "servercolon.h"
    #include "servercrlf.h"
    #include "usercmd.h"

    extern enum pop3_state_type 
    servercolon_feed(struct pop3_parser *p, uint8_t b) {
        struct servercolon_machine *mc = &p->state_machine.servercolon_mc;
        switch(mc->state) {
            case servercolon_start:
                if (b == '+') {
                    mc->state = servercolon_ok;
                } else if (b == '-') {
                    // En vez de esto podriamos redireccionar a una crlf machine
                    // mc->state = servercolon_error;
                    servercrlf_init(p);
                    return servercrlf;
                } else {
                    fprintf(stderr, "Unknown start of server response: %c\n", b);
                    abort();
                }
                break;
            case servercolon_error:
                fprintf(stderr, "Shouldnt be here, should be in crlf.\n");
                abort();
                break;
            case servercolon_ok:
                if (b == '\r') {
                    mc->state = servercolon_end1;
                }
                break;
            case servercolon_end1:
                if (b == '\n') {
                    mc->state = servercolon_end2;
                } else if (b != '\r') {
                    mc->state = servercolon_ok;
                }
                break;
            case servercolon_end2:
                if (b == '.') {
                    mc->state = servercolon_end3;
                } else if (b == '\r') {
                    mc->state = servercolon_end1;
                } else {
                    mc->state = servercolon_ok;
                }
                break;
            case servercolon_end3:
                if (b == '\r') {
                    mc->state = servercolon_end4;
                } else {
                    mc->state = servercolon_ok;
                }
                break;
            case servercolon_end4:
                if (b == '\n') {
                    usercmd_init(p);
                    return usercmd;
                } else if (b == '\r') {
                    mc->state = servercolon_end1;
                } else {
                    mc->state = servercolon_ok;
                }
                break;
            default:
                fprintf(stderr, "unknown state %d\n", p->state_name);
                abort();
        }
        return servercolon;

    }

    // Inicializacion de la maquina
    // Carga los punteros del parser a las distintas funciones
    // de la maquina, e inicializa variables internas
    void 
    servercolon_init(struct pop3_parser *p, bool retr) {
        struct servercolon_machine mc   = {
            .state              = servercolon_start,
            .transformations    = false,
        };
        p->state_name                   = servercolon;
        p->feed_method                  = servercolon_feed;
        p->state_machine.servercolon_mc     = mc;
    }
