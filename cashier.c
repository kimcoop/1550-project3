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

int clientsPresent() {

  return !empty( &shared->waiting_queue );

}

void signalClient() { 
  
  println("(CASHIER %d) waiting waiting_queue_mutex", cashier_id);

  p_sem_wait( &shared->waiting_queue_mutex );
  println("(CASHIER %d) recvd waiting_queue_mutex", cashier_id);
  client_id = dequeue( &shared->waiting_queue ); // signal to first client in queue
  println( "(CASHIER) client_id %d", client_id );
  shared->num_queued--;
  p_sem_post( &shared->waiting_queue_mutex );
  println("(CASHIER %d) post waiting_queue_mutex", cashier_id);

  // signal client that cashier will take him
  p_sem_post( &shared->cashier );
  println("(CASHIER %d) posted cashier is ready", cashier_id );

}

void serviceClient() {

  println("(CASHIER %d) waiting order placed ", cashier_id, client_id );
  p_sem_wait( &shared->ordered );
  println("(CASHIER %d) recvd order placed ", cashier_id, client_id );

  sleep( getRandTime( service_time ) );

  println("(CASHIER %d) waiting orders_mutex", cashier_id);
  p_sem_wait( &shared->orders_mutex );
  int item_id = shared->orders[ client_id ];
  println("(CASHIER %d) recvd orders mutex. client %d order: item_id %d", cashier_id, client_id, item_id );
  p_sem_post( &shared->orders_mutex );

  // log client's order in system
  p_sem_wait( &shared->db_mutex );
  FILE *fp = fopen( DB_FILE, "ab+" );
  fprintf( fp, DB_PRINT_FORMAT, client_id, item_id, getDescription( item_id ), getPrice( item_id ) );
  fprintf( fp, "\n" );
  fclose( fp );
  p_sem_post( &shared->db_mutex );

  // on successful payment, move client to waiting area (out of cashiers queue)
  println("(CASHIER %d) waiting order_queue_mutex ", cashier_id );
  p_sem_wait( &shared->order_queue_mutex );
  println("(CASHIER %d) recvd order_queue_mutex ", cashier_id );
  enqueue( &shared->order_queue, client_id ); // order_queue must populate before server gets called or will be nil
  p_sem_post( &shared->order_queue_mutex );
  println("(CASHIER %d) post order_queue_mutex ", cashier_id );

  // signal client the order went through
  println("(CASHIER %d) posting order placed ", cashier_id );
  p_sem_post( &shared->cashier_order_placed );

  println("(CASHIER %d) waiting payment ", cashier_id );
  p_sem_wait( &shared->payment );
  println("(CASHIER %d) recvd payment ", cashier_id );

  println("(CASHIER %d) posting receipt ", cashier_id );
  p_sem_post( &shared->receipt );

  // signal to server that there is a new order to be filled
  println("(CASHIER %d) posting new_order for server", cashier_id);
  p_sem_post( &shared->new_order ); 

  logOrder( item_id );
}

void logOrder( item_id ) {
  // log stats for client's order
  println("(CASHIER %d) waiting menu_items_mutex", cashier_id );
  p_sem_wait( &shared->menu_items_mutex );
  println("(CASHIER %d) recvd menu_items_mutexxxx", cashier_id );
  println("(CASHIER %d) increasing freq_menu_items[ %d ] from %d to %d", cashier_id, item_id-1, shared->freq_menu_items[ item_id-1 ], shared->freq_menu_items[ item_id-1 ]+1);
  shared->freq_menu_items[ item_id-1 ]++;
  p_sem_post( &shared->menu_items_mutex );
  println("(CASHIER %d) post menu_items_mutex", cashier_id );
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
  initSems();
  
  do {

    p_sem_wait( &shared->client_present );
      
    if ( !clientsPresent() ) {

      println( "(CASHIER %d) no clients present ", cashier_id );
      // sleep(  break_time );

    } else {

      signalClient();
      serviceClient();
      
    }
    
    println(" (CASHIER) shared->total_clients_served %d ", shared->total_clients_served );
  } while ( shared->total_clients_served < MAX_NUM_CLIENTS );

  println("( CASHIER )  detachSharedMem " );
  detachSharedMem( shared );


  return 0;
 
}
