/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

#include  "my_header.h"

int main( int argc, char *argv[] ) {
  
  int shmid;
  // if ( argc == 1 ) { 
  //   return 0;
  // } else if ( (argc-1) % 2 != 0 ) {
  //   println("Malformed flags.");
  //   return EXIT_FAILURE;
  // } else { 
    
    // int i;
    // char flag[ SMALL_BUFFER ];
    // for ( i = 1; i < argc; i++ ) {
    //   strcpy( flag, argv[ i ] );
    //   if ( strEqual(flag, "-h") ) 
        shmid = atoi( argv[ 1 ] );
    // }
  // }

  println( "shared memory segment ID: %d", shmid );
  println("");
  removeSharedMem( shmid );
  return 0;
 
}
