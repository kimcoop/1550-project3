/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_header.h"


int main( int argc, char *argv[] ) {

  int item_id = ITEM_ID, eat_time = EAT_TIME, max_people = MAX_PEOPLE, prob = PROBABILITY, shared_id = SHARED_ID;
  
  if ( argc == 1 ) {

    println("");
    println("Client Options:");
    println( " -i <int>\t menu ID of meal");
    println( " -e <int> \t max time client spends eating her food");
    println( " -m <int>\t max people in queue");
    println( " -p <int>\t probably with which client decides to leave");
    println( " -h <int> \t shared memory segment ID");
    println("");
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

  return 0;
 
}
