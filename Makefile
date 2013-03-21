
OBJS 	= cashier client server paner
CC	= gcc
FLAGS   = -o

all: cashier client server paner

# create/compile the individual files separately
cashier: cashier.c
	$(CC) $(FLAGS) cashier cashier.c

client: client.c
	$(CC) $(FLAGS) client client.c

server: server.c
	$(CC) $(FLAGS) server server.c

paner: paner.c
	$(CC) $(FLAGS) paner paner.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)