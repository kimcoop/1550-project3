/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/


void printValues();
void signalClient();
void serviceClient();
void logOrder();


#include  "my_header.h"

int service_time = SERVICE_TIME, 
    break_time = BREAK_TIME, 
    shared_id = SHARED_ID,
    cashier_id, 
    client_id;

void printValues() {
  println( "----" );
  println( "max time cashier takes to service client: %i", service_time );
  println( "max time cashier spends in break: %i", break_time );
  println( "shared memory segment ID: %i", shared_id );
  println( "----" );
  println( "" );
}

void signalClient() { 
  
  println("(CASHIER %d) waiting waiting_queue_mutex", cashier_id);

  sem_wait( &shared->waiting_queue_mutex );
  println("(CASHIER %d) recvd waiting_queue_mutex", cashier_id);
  client_id = dequeue( &shared->waiting_queue ); // signal to first client in queue
  shared->num_queued--;
  sem_post( &shared->waiting_queue_mutex );
  println("(CASHIER %d) post waiting_queue_mutex", cashier_id);

  sem_post( &shared->signal_client[client_id] ); // signal client that cashier will take him
  println("(CASHIER %d) signal_client cashier is ready", cashier_id);

}

void serviceClient() {

  println("(CASHIER %d) waiting client_ready_to_order ", cashier_id);
  // wait for called client to come to register
  sem_wait( &shared->client_ready_to_order );
  println("(CASHIER %d) received client_ready_to_order ", cashier_id);
  sleep( service_time );

  println("(CASHIER %d) waiting orders_mutex", cashier_id);
  sem_wait( &shared->orders_mutex );
  int item_id = shared->orders[ client_id ];
  println("(CASHIER %d) recvd orders mutex. client %d order: item_id %d", cashier_id, client_id, item_id );
  sem_post( &shared->orders_mutex );

  // log client's order in system
  sem_wait( &shared->db_mutex );
  FILE *fp = fopen( DB_FILE, "ab+" );
  fprintf( fp, DB_PRINT_FORMAT, client_id, item_id, getDescription( item_id ), getPrice( item_id ) );
  fprintf( fp, "\n" );
  fclose( fp );
  sem_post( &shared->db_mutex );

   // signal client the order went through
  println("(CASHIER %d) posting signal_client for order placed ", cashier_id);
  sem_post( &shared->signal_client[ client_id ] );

  logOrder();

  // signal to server that there is a new order to be filled
  
  println("(CASHIER %d) posting new_order for server", cashier_id);
  sem_post( &shared->new_order ); 

}

void logOrder( item_id ) {
  // log stats for client's order
  println("(CASHIER %d) waiting menu_items_mutex", cashier_id );
  sem_wait( &shared->menu_items_mutex );
  shared->freq_menu_items[ item_id-1 ]++;
  sem_post( &shared->menu_items_mutex );
}

int main( int argc, char *argv[] ) {

  
  if ( argc == 1 ) { 
    printCashierOptions();
    return 0;
  } else if ( (argc-1) % 2 != 0 ) {
    println("Malformed flags.");
    return EXIT_FAILURE;
  } else { // overwrite defaults
    
    int i;
    char flag[ SMALL_BUFFER ];
    for ( i = 1; i < argc; i++ ) {
      strcpy( flag, argv[ i ] );
      if ( strEqual(flag, "-s") ) 
        service_time = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-b") ) 
        break_time = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-h") ) 
        shared_id = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-u") ) 
        cashier_id = atoi( argv[ ++i ] );
    }
  }

  
  // printValues();
  shared = attachSharedMem( shared_id );

  do {

    println( "(if 0, cashier breaks) shared->num_queued = %d ", shared->num_queued );
    while ( shared->num_queued == 0 ) {
      #ifdef DEBUG
        sleep( 1 );
      #else
        sleep( break_time );
      #endif
      println( "CASHIER %d taking break ", cashier_id );
      println( "(0?) shared->num_queued = %d ", shared->num_queued );
    }

    signalClient();
    serviceClient();
    
  } while ( OPERATE );

  println("( CASHIER )  detachSharedMem " );
  detachSharedMem( shared );


  return 0;
 
}
