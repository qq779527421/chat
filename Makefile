CFLAGS := -DDEBUG -g -Wall

LDFLAGS := -lpthread  -lcrypt

OBJ := server client

all: $(OBJ)

server: server.o
	gcc -o $@ $^ $(LDFLAGS)
client: client.o
	gcc -o $@ $^ $(LDFLAGS)

%.o: %.c
	gcc $(INCLUDE) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *~ $(OBJ) *.o
