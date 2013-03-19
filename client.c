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
#include  <signal.h>
#include  "my_header.h"


int main( int argc, char *argv[] ) {

  int item_id = ITEM_ID, eat_time = EAT_TIME, max_people = MAX_PEOPLE, prob = PROBABILITY, shared_id = SHMID;
  
  if ( argc == 2 ) { // --help, --defaults

    if ( !strEqual( argv[1], "--help") ) {
      println("Options:");
      println( " -i <int>\t menu ID of meal");
      println( " -e <int> \t max time client spends eating her food");
      println( " -m <int>\t max people in queue");
      println( " -p <int>\t probably with which client decides to leave");
      println( " -s <int> \t shared memory segment ID");
      return 0;
    }

  } else if ( (argc-1) % 2 != 0 ) {
    println("Malformed flags.");
    return EXIT_FAILURE;

  } else { // overwrite defaults
    
    int i;
    char flag[ 2 ];
    for ( i = 1; i < argc; i += 2 ) {
      strcpy( flag, argv[ i ] );
      if ( strEqual(flag, "-i") ) 
        item_id = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-e") ) 
        eat_time = atoi( argv[ ++i ] );  
      else if ( strEqual(flag, "-m") ) 
        max_people = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-p") ) 
        prob = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-s") ) 
        shared_id = atoi( argv[ ++i ] );
    }
  }

  println( "menu ID of meal: %i", item_id );
  println( "max time client spends eating her food: %i", eat_time );
  println( "max people in queue: %i", max_people );
  println( "probably with which client decides to leave: %i", prob );
  println( "shared memory segment ID: %i", shared_id );
  println("");

  return 0;
 
}
