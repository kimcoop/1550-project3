/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

#include  "my_header.h"

int main( int argc, char *argv[] ) { // TODO: parse SHMIDS.TXT and iteratively cleanup
  
  int shmid, i;
  if ( argc-1 == 0 ) {
    println( "Usage: ./cleanup <shmid1> <shmid2> ..." );
  } else {
    for ( i = 1; i <= argc-1; i++ ) {
      shmid = atoi( argv[ i ] );
      println( "Cleaning up shmid: %d", shmid );
      shared = attachSharedMem( shmid );
      destroySems();
      detachSharedMem( shared );
      removeSharedMem( shmid );
    }

  }

  return 0;
 
}
