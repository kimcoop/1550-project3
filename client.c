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
    shared_id = SHARED_ID;

void arrive() {
  println("[CLIENT %d] arrive ", client_id);
  // line up in FIFO queue awaiting chance to give order.
  // if there are more than max_people already queued, client decides to leave with probability prob.
  if ( shared->num_queued >= max_people ) {
    println(" max_people in queue ");
    exit( 0 );
    // enter queue with 1-prob 
  } else {
    // enter queue

    println("[CLIENT %d] waiting waiting_queue_mutex ", client_id);

    sem_wait( &shared->waiting_queue_mutex );
    println("[CLIENT %d] recvd waiting_queue_mutex ", client_id);
    enqueue( &shared->waiting_queue, client_id ); // waiting to place order
    shared->num_queued++;

    println("[CLIENT %d ] shared->num_queued = %d", client_id, shared->num_queued);
    sem_post( &shared->waiting_queue_mutex );
    println("[CLIENT %d] post waiting_queue_mutex ", client_id);

  }

}

void waitForCashier() {

  println("[CLIENT %d] waiting for cashier to signal ", client_id);
  sem_wait( &shared->signal_client[client_id] );
  sem_wait( &shared->cashier_ready );
  println("[CLIENT %d] received cashier signal ", client_id);

  sem_post( &shared->signal_client[ client_id ] );
  sem_post( &shared->client_ready_to_order );
  println("[CLIENT %d] posted ready to order ", client_id);
  
}

void order() {
  // consists of a single item. client then proceeds to order queue

  println("[CLIENT %d] waiting orders_mutex", client_id );
  sem_wait( &shared->orders_mutex );
  println("[CLIENT %d] recv orders_mutex. ordering item_id %d", client_id, item_id );
  shared->orders[ client_id ] = item_id;
  sem_post( &shared->orders_mutex );
  println("[CLIENT %d] posting orders_mutex", client_id );

}

void pay() {
  
  println("[CLIENT %d] waiting for cashier to signal order placed ", client_id );
  sem_wait( &shared->signal_client[ client_id ] );
  sem_wait( &shared->cashier_order_placed );
  println("[CLIENT %d] received cashier to signal order placed ", client_id );

  println("[CLIENT %d] paying", client_id );

}


void waitForFood() {
  // wait pseudo-random amount of time between rand(min) and rand(max) for item_id
  
  int wait_time = getWaitTime( item_id );
  println("[CLIENT %d] waiting for food ", client_id );
  
  #ifdef DEBUG
    sleep( 1 );
  #else
    sleep( wait_time );
  #endif

  sem_wait( &shared->signal_client[ client_id ] );

  println("[CLIENT] waiting server_dispatch_ready");
  sem_wait( &shared->server_dispatch_ready );
  println("[CLIENT] recvd server_dispatch_ready");
  

}

void getFood() {

  //TODO - may need mutex here
  shared->total_clients_served++;

  sem_wait( &shared->order_queue_mutex );
  // TODO - we need some other structure to hold these since
  int c_id = dequeue( &shared->order_queue ); // the first client in the order_queue MAY NOT BE the client_id. circle back
  println("**[CLIENT] issue if not matching: %d & %d ", client_id, c_id );
  sem_post( &shared->order_queue_mutex );

}

void eat() {
  //
  println("[CLIENT %d] eating", client_id );

  #ifdef DEBUG
    sleep( 1 );
  #else
    sleep( eat_time );
  #endif

}

void leave() {
  
  sem_wait( &shared->client_exit_mutex );
  println("[CLIENT %d] leaving ", client_id );
  shared->num_exited++;

  println("[CLIENT %d] shared->num_exited %d ", client_id, shared->num_exited);
  sem_post( &shared->client_exit_mutex );

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

  if ( argc == 1 ) {
    printClientOptions();
    return 0;
  } else if ( (argc-1) % 2 != 0 ) {
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
    item_id = rand() % 21;
  if ( client_id == CLIENT_ID ) {
    println( "Error: client ID not passed into client. " );
    exit( EXIT_FAILURE );
  }

  // printValues();
  
  shared = attachSharedMem( shared_id );

  // if (sem_wait( &shared->cashier_ready ) == -1) {
  //   perror( "sem_wait ");
  // }

  arrive();
  waitForCashier();
  order();
  pay();
  waitForFood();
  getFood();
  eat();
  leave();

  println( "[CLIENT] %d detaching", client_id );
  detachSharedMem( shared );

  exit( 0 );
 
}
