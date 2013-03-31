/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

#include  "my_header.h"

void cleanupShmids( char* );
void cleanupShmid( int );


void cleanupShmids( char* filename ) {

  int shmid;
  char c;
  FILE  *fp = NULL;

  if (( fp = fopen( filename, "r" )) == NULL ) {
    println( "Unknown file %s", filename );
    exit( 1 );
  } else {
    while ( !feof(fp)  ) {
      fscanf( fp, "%d %c", &shmid, &c );
      cleanupShmid( shmid );
    } // end while 
    fclose(fp);
  }
}

void cleanupShmid( int shmid ) {
  println( "Cleaning up shmid %d.", shmid );
  shared = attachSharedMem( shmid );
  markShmemForRemoval( shmid );
  destroySems();
  detachSharedMem( shared );
}

int main( int argc, char *argv[] ) {
  
  int shmid, i;
  if ( argc-1 == 0 ) {
    println( "Cleanup usage: ");
    println( "./cleanup <shmid1> <shmid2> ..." );
    println( "Or to scrape shmids from %s, run: ", CLEANUP_FILE );
    println( "./cleanup all" );
  } else {

    if ( (argc-1 == 1) && strEqual( argv[1], "all" ) ) {
      println( "Cleaning up semaphores listed in file %s", CLEANUP_FILE );
      cleanupShmids( CLEANUP_FILE ); // clean up from file
      emptyFile( CLEANUP_FILE ); // clear file
    } else {

      for ( i = 1; i <= argc-1; i++ ) { // clean up via command line args
        shmid = atoi( argv[ i ] );
        cleanupShmid( shmid );
      }
      
    }
  }
  return 0;
}
