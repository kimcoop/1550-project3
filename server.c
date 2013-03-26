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

  if ( !empty( &shared->order_queue ) ) {
    client_id = peek( &shared->order_queue );
    println("[-SERVER-] prepareFood for client_id %d", client_id );
  }

  sem_post( &shared->order_queue_mutex );
  sem_post( &shared->food_prepared );

}

void serveFood() {
  
  sem_wait( &shared->food_prepared );
  sem_wait( &shared->order_queue_mutex );

  if ( !empty( &shared->order_queue ) ) {
    client_id = peek( &shared->order_queue );
    println("[-SERVER-] serveFood for client_id %d  ", client_id );
    shared->food_ready_client_id = client_id;
    println("[-SERVER-] shared->food_ready_client_id %d ", shared->food_ready_client_id );

    sem_post( &shared->food_ready );
  }
  sem_post( &shared->order_queue_mutex );
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

  installSignalHandler();

  // printValues();
  shared = attachSharedMem( shared_id );
  do {
    prepareFood();
    serveFood();
  } while ( shared->num_queued > 0 || shared->num_exited == 0 );

  // println("[-SERVER-]  detachSharedMem " );
  // detachSharedMem( shared );
  
  return 0;
 
}
