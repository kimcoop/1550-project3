
OBJS 	= cashier client server paner cleanup
CC	= gcc
LINKING_FLAGS = -lrt
OTHER_FLAGS = -o

all: cashier client server paner cleanup
mine: cashier_m client_m server_m paner_m cleanup_m

### thot flags
cashier: cashier.c
	$(CC) $(LINKING_FLAGS) cashier.c $(OTHER_FLAGS) cashier

client: client.c
	$(CC) $(LINKING_FLAGS) client.c $(OTHER_FLAGS) client

server: server.c
	$(CC) $(LINKING_FLAGS) server.c $(OTHER_FLAGS) server

paner: paner.c
	$(CC) $(LINKING_FLAGS) paner.c $(OTHER_FLAGS) paner

cleanup: cleanup.c
	$(CC) $(LINKING_FLAGS) cleanup.c $(OTHER_FLAGS) cleanup

###### local flags (modified)
cashier_m: cashier.c
	$(CC) cashier.c $(OTHER_FLAGS) cashier

client_m: client.c
	$(CC) client.c $(OTHER_FLAGS) client

server_m: server.c
	$(CC) server.c $(OTHER_FLAGS) server

paner_m: paner.c
	$(CC) paner.c $(OTHER_FLAGS) paner

cleanup_m: cleanup.c
	$(CC) cleanup.c $(OTHER_FLAGS) cleanup

# clean house
clean:
	rm -f $(OBJS) $(OUT)