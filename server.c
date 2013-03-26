/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

/*
  void printValues();
  void prepareFood();
  void serveFood();
*/

#include  "my_header.h"

int service_time = SERVICE_TIME,
    shared_id = SHARED_ID, 
    client_id;


void printValues() {

  println( "-------");
  println( "max time server takes to service client: %i", service_time );
  println( "shared memory segment ID: %i", shared_id );
  println( "-------");

}

void prepareFood() {

  sem_wait( &shared->new_order );
  println("{ SERVER! } new_order");
  sem_wait( &shared->order_queue_mutex );

  if ( !empty( &shared->order_queue ) ) {
    client_id = peek( &shared->order_queue );
    println("{ SERVER } prepareFood for client_id %d", client_id );
    println("{ SERVER } serveFood for client_id %d  ", client_id );
    sem_post( &shared->order_up[client_id] );
  } else {
    println("ORDER QUEUE EMPTY");
  }

  sem_post( &shared->order_queue_mutex );

}

void serveFood() {
  sem_post( &shared->server_dispatch_ready );
  println("{ SERVER } server_dispatch_ready");
}

int main( int argc, char *argv[] ) {

  
  if ( argc == 1 ) { 
    printServerOptions();
    return 0;
  } else if ( (argc-1) % 2 != 0 ) {
    println("Malformed flags.");
    return EXIT_FAILURE;
  } else { // overwrite defaults
    
    int i;
    char flag[ SMALL_BUFFER ];
    for ( i = 1; i < argc; i++ ) {
      strcpy( flag, argv[ i ] );
      if ( strEqual(flag, "-t") ) 
        service_time = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-h") ) 
        shared_id = atoi( argv[ ++i ] );
    }
  }

  // printValues();
  shared = attachSharedMem( shared_id );
  int i = 0;
  do {
    prepareFood();
    serveFood();
    i++;
  } while ( i < MAX_NUM_CLIENTS && OPERATE );

  println("{ SERVER }  detachSharedMem " );
  detachSharedMem( shared );
  
  return 0;
 
}
