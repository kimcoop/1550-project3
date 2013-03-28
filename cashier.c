/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

/*
void printValues();
void signalClient();
void serviceClient();
*/

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
  
  sem_wait( &shared->waiting_queue_mutex );
  client_id = dequeue( &shared->waiting_queue );
  sem_post( &shared->waiting_queue_mutex );

  sem_post( &shared->signal_client[client_id] ); // signal client that cashier will take him

}

void serviceClient() {
  sem_wait( &shared->client_ready_to_order );
  println("(CASHIER %d) received client_ready_to_order ", cashier_id);
  sleep( service_time );

  sem_wait( &shared->orders_mutex );
  int item_id = shared->orders[ client_id ];
  println("(CASHIER %d) received client %d order: item_id %d", cashier_id, client_id, item_id );
  sem_post( &shared->orders_mutex );

  // log client's order in system
  sem_wait( &shared->db_mutex );
  FILE *fp = fopen( DB_FILE, "ab+" );
  fprintf( fp, DB_PRINT_FORMAT, client_id, item_id, getDescription( item_id ), getPrice( item_id ) );
  fprintf( fp, "\n" );
  fclose( fp );
  sem_post( &shared->db_mutex );

  // log stats for client's order
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

  int i = 0;
  do {

    if ( empty( &shared->waiting_queue ) ) {
      sleep( break_time );
      println( "CASHIER %d taking break ", cashier_id );
    }

    signalClient();
    serviceClient();
    
    i++;

  } while ( i < MAX_NUM_CLIENTS && OPERATE );

  println("( CASHIER )  detachSharedMem " );
  detachSharedMem( shared );


  return 0;
 
}
