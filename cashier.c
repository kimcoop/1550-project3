/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/


void signalClient();
void serviceClient();
void logOrder();


#include  "my_header.h"

int service_time = SERVICE_TIME, 
    break_time = BREAK_TIME, 
    shared_id = SHARED_ID,
    client_id;


int clientsPresent() {

  return !empty( &shared->waiting_queue );

}

void signalClient() { 
  
  p_sem_wait( &shared->waiting_queue_mutex );
  client_id = dequeue( &shared->waiting_queue ); // signal to first client in queue
  shared->num_queued--;
  p_sem_post( &shared->waiting_queue_mutex );

  // signal client that cashier will take him
  p_sem_post( &shared->cashier );

}

void serviceClient() {

  p_sem_wait( &shared->ordered );
  println( "A cashier is speaking with client %d. ", client_id );

  sleep( getRandTime( service_time ) );
  p_sem_wait( &shared->orders_mutex );

  int item_id = shared->orders[ client_id ];
  println( "A cashier is placing order (item %d) for client %d.", item_id, client_id );
  p_sem_post( &shared->orders_mutex );

  // log client's order in system
  p_sem_wait( &shared->db_mutex );
  FILE *fp = fopen( DB_FILE, "ab+" );
  fprintf( fp, DB_PRINT_FORMAT, client_id, item_id, getDescription( item_id ), getPrice( item_id ) );
  fprintf( fp, "\n" );
  fclose( fp );
  p_sem_post( &shared->db_mutex );

  // on successful payment, move client to waiting area (out of cashiers queue)
  p_sem_wait( &shared->order_queue_mutex );
  enqueue( &shared->order_queue, client_id ); // order_queue must populate before server gets called or will be nil
  p_sem_post( &shared->order_queue_mutex );

  // signal client the order went through
  p_sem_post( &shared->cashier_order_placed );

  log( "A cashier is awaiting payment from client %d.", client_id );
  p_sem_wait( &shared->payment );
  log( "A cashier has received payment from client %d.", client_id );

  log( "A cashier is posting receipt for client %d.", client_id );
  p_sem_post( &shared->receipt );

  // signal to server that there is a new order to be filled
  log( "A cashier is posting client %d's new_order for server.", client_id );
  p_sem_post( &shared->new_order ); 

  logOrder( item_id );
}

void logOrder( item_id ) {
  // log stats for client's order
  p_sem_wait( &shared->menu_items_mutex );
  shared->freq_menu_items[ item_id-1 ]++;
  p_sem_post( &shared->menu_items_mutex );
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
    }
  }

  shared = attachSharedMem( shared_id );
  initSems();
  int i = 0;
  
  do {

    p_sem_wait( &shared->client_present );
      
    if ( !clientsPresent() ) {

      println( "No clients present. Cashier taking break." );
      sleep(  break_time );

    } else {

      signalClient();
      serviceClient();
      
    }

    i++;
    
  } while ( i < MAX_NUM_CLIENTS );

  log( "A cashier detaching from shared memory." );
  detachSharedMem( shared );

  return 0;
 
}
