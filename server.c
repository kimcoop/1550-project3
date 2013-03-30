/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

/*
  void awaitOrder();
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

void awaitOrder() {

  p_sem_wait( &shared->new_order );
  println( "SERVER received a new order." );
  
}

void prepareFood() {
  p_sem_wait( &shared->order_queue_mutex );

  if ( !empty( &shared->order_queue ) ) {
    client_id = dequeue( &shared->order_queue );
    println("SERVER prepared food for client_id %d", client_id );
    p_sem_post( &shared->signal_client[ client_id ] );
  } else {
    println("ORDER QUEUE EMPTY");
  }

  p_sem_post( &shared->order_queue_mutex );
}

void serveFood() {

  sleep( getRandTime( service_time ) );

  // wait for client to come to server table
  println("{ SERVER } recvd client %d at server table", client_id );
  p_sem_wait( &shared->signal_client[ client_id ] );

  // hand over meal
  println("{ SERVER } posting meal dispatched to client %d ", client_id );
  p_sem_post( &shared->meal_dispatch );

}

int main( int argc, char *argv[] ) {

  
  if ( (argc-1) % 2 != 0 ) {
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
  initSems();

  do {
    awaitOrder();
    prepareFood();
    serveFood();
    println("{{ SERVER }} shared->total_clients_served %d ", shared->total_clients_served );
  } while ( shared->total_clients_served < MAX_NUM_CLIENTS );

  println("{ SERVER }  detachSharedMem " );
  detachSharedMem( shared );
  
  return 0;
 
}
