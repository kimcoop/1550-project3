/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

/*
void client();
void cashier();
void server();
void launch();
*/

#include "my_header.h"

void client() {
  if ( execl( "./client",  "client", "-h", shmid_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void cashier() {
  if ( execl( "./cashier",  "cashier", "-h", shmid_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void server() {
  if ( execl( "./server",  "server", "-h", shmid_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void launch() {

  pid_t child_pid;
  int i=0, j=0;
  if ( (child_pid = fork() ) < 0 ) {
    perror("fork"); 
    exit( EXIT_FAILURE );
  } else if ( child_pid == 0 ) {
    while ( 1 && i < 4 ) {
      
      client();
      i++;

    }
  } else { // parent
    while ( 1  && j < 4 ) {
      sem_wait( &shared.mutex );
      println(" parent %d acquiring mutex ", getpid() );
      log(" (parent) shared contents: %s\n", data);
      strncpy( data, "parent! ", SHM_SIZE );
      sem_post( &shared.mutex );
      j++;
    }
  }
  
  println( "shmid: %d ", shmid );

}

int main( int argc, char *argv[] ) {

  int num_cashiers = NUM_CASHIERS; // TODO - ensure we pass other args into exec calls
  
  if ( argc == 1 ) {
    printClientOptions();
    printServerOptions();
    printCashierOptions();
    return 0;
  } else if ( argc == 2 ) { // --defaults, -d, etc
    println("Using defaults.");
    USE_DEFAULTS = TRUE;
  } else if ( (argc-1) % 2 != 0 ) {
    println("Malformed flags.");
    return EXIT_FAILURE;
  } else { // overwrite defaults
    
    int i;
    char flag[ SMALL_BUFFER ];

    for ( i = 1; i < argc; i++ ) {
      strcpy( flag, argv[ i ] );
      if ( strEqual(flag, "-k") )
        num_cashiers = atoi( argv[ ++i ] );
    }
  }

  int parent_id = getpid(); // gather while we know we are parent (only) process
  initSems();
  key = ftok( KEY, KEY_MODE ); 
  shmid = allocateSharedMem( key );
  data = attachSharedMem( shmid );
  
  launch();

  println("waiting");
  wait( NULL ); // wait all child processes
  println("done waiting");
  if ( getpid() == parent_id ) { // clean up after all child processes have exited
    detachSharedMem( data );
    removeSharedMem( shmid );
  }

  return 0;
 
}
