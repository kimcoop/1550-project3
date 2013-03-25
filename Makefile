
OBJS 	= cashier client server paner cleanup
CC	= gcc
FLAGS   = -o

all: cashier client server paner cleanup

# create/compile the individual files separately
cashier: cashier.c
	$(CC) $(FLAGS) cashier cashier.c

client: client.c
	$(CC) $(FLAGS) client client.c

server: server.c
	$(CC) $(FLAGS) server server.c

paner: paner.c
	$(CC) $(FLAGS) paner paner.c

cleanup: cleanup.c
	$(CC) $(FLAGS) cleanup cleanup.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)