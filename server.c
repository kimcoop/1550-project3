/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

/*
  void awaitOrder();
  void prepareFood();
  void serveFood();
*/

#include  "my_header.h"

int service_time = SERVICE_TIME,
    shared_id = SHARED_ID, 
    client_id, item_id;


void awaitOrder() {

  p_sem_wait( &shared->new_order );
  println( "Server received a new order." );
  
}

void prepareFood() {
  p_sem_wait( &shared->order_queue_mutex );

  if ( !empty( &shared->order_queue ) ) {
    client_id = dequeue( &shared->order_queue );
    item_id = shared->orders[ client_id ];
    println( "Server preparing order (item_id %d) for client %d.", item_id, client_id );
    p_sem_post( &shared->signal_client[ client_id ] );
  } else {
    println( "Error: ORDER QUEUE EMPTY." );
  }

  p_sem_post( &shared->order_queue_mutex );
}

void serveFood() {

  sleep( getRandTime( service_time ) );

  // wait for client to come to server table
  log( "Server waiting for client %d at server table.", client_id );
  p_sem_wait( &shared->signal_client[ client_id ] );

  // hand over meal
  println( "Server handing meal to client %d.", client_id );
  p_sem_post( &shared->signal_client[ client_id ] );
  // p_sem_post( &shared->meal_dispatch );

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

  shared = attachSharedMem( shared_id );
  initSems();
  int i = 0;

  do {

    awaitOrder();
    prepareFood();
    serveFood();
    i++;

  } while ( i < MAX_NUM_CLIENTS );

  log( "Server detaching from shared memory." );
  detachSharedMem( shared );
  
  return 0;
 
}
