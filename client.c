/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

/*


void arrive();
void order();
void pay();
void getReceipt();
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
  println("[CLIENT] arrive ");
  // line up in FIFO queue awaiting chance to give order.
  // if there are more than max_people already queued, client decides to leave with probability prob.
  if ( shared->num_queued >= max_people ) {
    println(" max_people in queue ");
    // enter queue with 1-prob 
  } else {
    // enter queue

    // sem_wait( &shared->mutex );
    // log("[CLIENT] shared->data: %s", shared->data);
    // strncpy( shared->data, "child! ", SMALL_BUFFER );
    // sem_post( &shared->mutex );

    sem_wait( &shared->waiting_queue_mutex );
    enqueue( &shared->waiting_queue, client_id );
    shared->num_queued++;

    println("[CLIENT] shared->num_queued = %d", shared->num_queued);
    sem_post( &shared->waiting_queue_mutex );



  }

}

void order() {
  // consists of a single item.
  // client proceeds to waiting queue.
  // TODO - item_id persists
  println("[CLIENT] ordering (%d) ", client_id );
  sem_wait( &shared->order_queue_mutex );
  sem_wait( &shared->waiting_queue_mutex );

  dequeue( &shared->waiting_queue ); // returns client_id
  enqueue( &shared->order_queue, client_id );
  shared->num_queued--;
  println("[CLIENT] shared->num_queued = %d", shared->num_queued);


  sem_post( &shared->waiting_queue_mutex );
  sem_post( &shared->order_queue_mutex );

  sem_post( &shared->new_order );

}

void pay() {
  //
  println("[CLIENT] pay()" );

}

void getReceipt() {
  //
  println("[CLIENT] getReceipt() ");
}

void getFood() {
  //
  println("[CLIENT] getFood() ");
}

void eat() {
  //
  println("[CLIENT] eat() ");
}

void leave() {
  //
  println("[CLIENT] leave() ");
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
    }
  }

  if ( item_id == ITEM_ID )
    item_id = rand() % 21;

  // printValues();
  
  shared = attachSharedMem( shared_id );
  srand( time(NULL) );

  client_id = getpid();

  arrive();
  order();


  detachSharedMem( shared );

  return 0;
 
}
