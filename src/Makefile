CC=gcc
CFLAGS = -std=c99 --pedantic -Werror -Wall -I.
DEPS= pop3.h usercmd.h servercolon.h servercrlf.h buffer.h
OBJ= pop3.o  usercmd.o servercolon.o servercrlf.o buffer.o

p3filter.pt: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ): $(OBJ:.o=.c) $(DEPS) 
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

clean:
	rm -rf $(OBJ)
