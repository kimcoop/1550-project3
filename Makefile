
OBJS 	= cashier client server
CC	= gcc
FLAGS   = -o

all: cashier client server

# create/compile the individual files >>separately<< 
cashier: cashier.c
	$(CC) $(FLAGS) cashier cashier.c

client: client.c
	$(CC) $(FLAGS) client client.c

server: server.c
	$(CC) $(FLAGS) server server.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)