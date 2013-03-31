/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

/*


void arrive();
waitForCashier();
void order();
void storeOrder();
void pay();
void waitForFood();
void getFood();
void eat();
void leave();

void printValues();


*/

#include "my_header.h"

int client_id, 
    item_id = ITEM_ID, 
    eat_time = EAT_TIME, 
    max_people = MAX_PEOPLE, 
    prob = PROBABILITY, 
    shared_id = SHARED_ID,
    time_in_shop, // ms
    time_waiting; // ms
clock_t time_in,
        time_out;


void arrive() {
  // enter queue

  p_sem_post( &shared->client_present );

  p_sem_wait( &shared->waiting_queue_mutex );
  enqueue( &shared->waiting_queue, client_id ); // waiting to place order

  shared->num_queued++;

  p_sem_post( &shared->waiting_queue_mutex );

  time_in = clock();

}

void waitForCashier() {

  println(" [CLIENT %d] waiting on cashier ");
  p_sem_wait( &shared->cashier );
  println(" [CLIENT %d] with cashier ");
  
}

void order() {
  // consists of a single item. client then proceeds to order queue

  p_sem_wait( &shared->orders_mutex );
  shared->orders[ client_id ] = item_id;
  p_sem_post( &shared->orders_mutex );

  println("Client %d ordered.", client_id );
  p_sem_post( &shared->ordered );

}

void pay() {
  
  p_sem_wait( &shared->cashier_order_placed );
  println("Client %d is paying.", client_id );
  p_sem_post( &shared->payment );
  p_sem_wait( &shared->receipt );

}


void waitForFood() {
  // wait pseudo-random amount of time between rand(min) and rand(max) for item_id
  
  int wait_time = getWaitTime( item_id );
  sleep( getRandTime( wait_time ) );  
  p_sem_wait( &shared->signal_client[ client_id ] );

}

void getFood() {

  // signal to server that client is at server table
  p_sem_post( &shared->signal_client[ client_id ] );

  // wait for server to hand over meal
  p_sem_wait( &shared->meal_dispatch );
  println("Client %d received meal from server.", client_id );

  shared->total_clients_served++;

}

void eat() {
  //
  println("Client %d is eating.", client_id );
  sleep( getRandTime( eat_time ) );

}

void leave() {
  
  p_sem_wait( &shared->client_exit_mutex );
  println("Client %d is exiting.", client_id );
  shared->num_exited++;
  
  p_sem_post( &shared->client_exit_mutex );
  
  time_out = clock();
  int seconds =  ( time_out - time_in ) / CLOCKS_PER_SEC;
  // println( "******** TOTAL TIME(ms)  IN SHOP FOR CLIENT %d IS %d ", client_id, seconds ); // tODO

}


void printValues() {
  println( "-------");
  println( "menu ID of meal: %d", item_id );
  println( "max time client spends eating her food: %d", eat_time );
  println( "max people in queue: %d", max_people );
  println( "probably with which client decides to leave: %d", prob );
  println( "shared memory segment ID: %d", shared_id );
  println( "-------");
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
      if ( strEqual(flag, "-i") ) 
        item_id = atoi( argv[++i] );
      else if ( strEqual(flag, "-e") ) 
        eat_time = atoi( argv[++i] );  
      else if ( strEqual(flag, "-m") ) 
        max_people = atoi( argv[++i] );
      else if ( strEqual(flag, "-p") ) 
        prob = atoi( argv[++i] );
      else if ( strEqual(flag, "-h") ) 
        shared_id = atoi( argv[++i] );
      else if ( strEqual(flag, "-x") )
        client_id = atoi( argv[++i] );
    }
  }

  srand( time(NULL) );
  if ( item_id == ITEM_ID )
    item_id = (rand() % 20) + 1; // ensure not 0
  if ( client_id == CLIENT_ID ) {
    println( "Error: client ID not passed into client. " );
    exit( EXIT_FAILURE );
  }

  // printValues();
  
  shared = attachSharedMem( shared_id );
  initSems();

  println("[CLIENT %d] arrive ", client_id);

  // if there are more than max_people already queued, client decides to leave with probability prob.
  if ( shared->num_queued >= max_people ) { // enter queue with 1-prob
    println(" max_people in queue ");
  } else {

    arrive();
    waitForCashier();
    order();
    pay();
    waitForFood();
    getFood();
    eat();
    leave();
    
  }

  println( "[CLIENT] %d detaching", client_id );
  detachSharedMem( shared );

  exit( 0 );
 
}
