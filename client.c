/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

/*


void doubleQueueSize();
void stuff();
void arrive( int );
void order( int );

*/

#include "my_header.h"

int item_id = ITEM_ID, 
    eat_time = EAT_TIME, 
    max_people = MAX_PEOPLE, 
    prob = PROBABILITY, 
    shared_id = SHARED_ID;


void arrive( int client_id ) {
  println(" arrive ");
  // line up in FIFO queue awaiting chance to give order.
  // if there are more than max_people already queued, client decides to leave with probability prob.
  if ( shared->num_queued >= max_people ) {
    println(" max_people in queue ");
    // enter queue with 1-prob 
  } else {
    // enter queue
    sem_wait( &shared->waiting_queue );
    enqueue( shared->queued_clients,  client_id );
    shared->num_queued++;
    println(" shared->num_queued = %d", shared->num_queued);
    sem_post( &shared->waiting_queue );

  }

}

void order( int client_id ) {
  // consists of a single item.
  // client proceeds to waiting queue.
  

  
  println(" menu item is %d ", item_id );

}


void stuff() {

  sem_wait( &shared->waiting_queue ); // if no waiting_queue slots, wait
  println(" CLIENT- child %d acquiring waiting_queue mutex ", getpid() );
  sem_post( &shared->waiting_queue );
  shared->total_clients++;
  println( "shared->total_clients = %d", shared->total_clients);

  char str[ SMALL_BUFFER ] = "Child ID is ";
  strncat( str, "test", SMALL_BUFFER );
  strncpy( shared->data, "test", SMALL_BUFFER );
  println( "shared->data = %s", shared->data);

  writeToFile( OUTPUTFILE, str );
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

  println("-------");
  println( "menu ID of meal: %d", item_id );
  println( "max time client spends eating her food: %d", eat_time );
  println( "max people in queue: %d", max_people );
  println( "probably with which client decides to leave: %d", prob );
  println( "shared memory segment ID: %d", shared_id );
  println("-------");
  
  shared = attachSharedMem( shared_id );
  srand( time(NULL) );

  int client_id = getpid();
  arrive( client_id );

  detachSharedMem( shared );
  return 0;
 
}
