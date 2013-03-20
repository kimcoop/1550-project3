/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/


#include  "my_header.h"


int main( int argc, char *argv[] ) {

  int service_time = SERVICE_TIME, break_time = BREAK_TIME, shared_id = SHARED_ID;
  
  if ( argc == 1 ) { 

    println("");
    println("Cashier Options:");
    println( " -s <int>\t max time cashier takes to service client" );
    println( " -b <int> \t max time cashier spends in break" );
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
      if ( strEqual(flag, "-s") ) 
        service_time = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-b") ) 
        break_time = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-h") ) 
        shared_id = atoi( argv[ ++i ] );
    }
  }

  println( "max time cashier takes to service client: %i", service_time );
  println( "max time cashier spends in break: %i", break_time );
  println( "shared memory segment ID: %i", shared_id );
  println("");

  return 0;
 
}
