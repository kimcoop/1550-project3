/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/


#include "my_header.h"

void stuff() {
  println(" CHILD PROCESS- CLIENT ");
  sem_wait( &shared.empty ); // if no empty slots, wait
  sem_wait( &shared.mutex ); // if another is using buffer, wait
  println(" CLIENT- child %d acquiring mutex ", getpid() );
  log(" (child) shared contents: %s\n", data);
  strncpy( data, "child! ", SHM_SIZE );
  sem_post( &shared.mutex );
  sem_post( &shared.full );
}

int main( int argc, char *argv[] ) {

  int item_id = ITEM_ID, eat_time = EAT_TIME, max_people = MAX_PEOPLE, prob = PROBABILITY, shared_id = SHARED_ID;
  
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

  println( "menu ID of meal: %d", item_id );
  println( "max time client spends eating her food: %d", eat_time );
  println( "max people in queue: %d", max_people );
  println( "probably with which client decides to leave: %d", prob );
  println( "shared memory segment ID: %d", shared_id );
  println("");
  stuff();

  return 0;
 
}
