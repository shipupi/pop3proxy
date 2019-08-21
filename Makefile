CC=gcc
CFLAGS = -Wall -I.
DEPS= pop3.h usercmd.h 
OBJ= pop3.o  usercmd.o

p3filter.pt: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ): $(OBJ:.o=.c) $(DEPS) 
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)

clean:
	rm -rf $(OBJ)
