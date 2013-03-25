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
  sem_wait( &shared->order_queue_mutex );
  if ( !empty( &shared->order_queue ) ) { // TODO - this shouldn't be needed
    println("[SERVER] prepareFood() ");
    client_id = dequeue( &shared->order_queue );
    println("[SERVER] servicing order for client_id %d", client_id );
  }
  sem_post( &shared->order_queue_mutex );
  sem_post( &shared->food_ready );

}

void serveFood() {
  sem_wait( &shared->food_ready );
  println("[SERVER] serveFood() for client_id %d  ", client_id);

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

  while ( i < 10 ) {
    prepareFood();
    serveFood();
    i++;
  }

  detachSharedMem( shared );

  
  return 0;
 
}
