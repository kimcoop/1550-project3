/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

/*
void cleanup( char* );
void client( char* );
void cashier( char* );
void server( char* );
void openDoors();
void initSharedData();
void initSharedMem();
*/

#include "my_header.h"

key_t key;
int shmid;



void cleanup( char* shmid_str ) {
  println(" shmid_str is %s ", shmid_str );
  if ( execl( "./cleanup",  "cleanup", shmid_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void client( char* shmid_str ) {
  println(" shmid_str is %s ", shmid_str );
  if ( execl( "./client",  "client", "-h", shmid_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void cashier( char* shmid_str ) {
  if ( execl( "./cashier",  "cashier", "-h", shmid_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void server( char* shmid_str) {
  if ( execl( "./server",  "server", "-h", shmid_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void openDoors() {
  
  char shmid_str[ SMALL_BUFFER ];
  toString( shmid_str, shmid );

  pid_t child_pid;
  int i=0, j=0;
  if ( (child_pid = fork() ) < 0 ) {
    perror("fork"); 
    exit( EXIT_FAILURE );
  } else if ( child_pid == 0 ) {
    while ( i < 4 ) {
      
      client( shmid_str );
      i++;

    }
  } else { // parent
    while ( j < 4 ) {
      sem_wait( &shared->mutex );
      log("[PARENT] shared->data: %s", shared->data);
      strncpy( shared->data, "parent! ", SMALL_BUFFER );
      sem_post( &shared->mutex );
      j++;
    }
  }
  
  println( "shmid: %d ", shmid );

}

void initSharedData() {
  shared->total_clients_served = 0;
  shared->num_queued = 0;
  shared->total_revenue = 0.0;
  shared->total_wait_time = 0;
  init_queue( &shared->waiting_queue );
}

void initSharedMem() {
  key = ftok( KEY, KEY_MODE ); 
  shmid = allocateSharedMem( key );
  shared = attachSharedMem( shmid );
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
  setbuf( stdout, NULL ); // stdout is unbuffered

  initSems();
  initSharedMem();
  initSharedData();
  openDoors();

  println("waiting");
  wait( NULL ); // wait all child processes
  println("done waiting");

  println( "[PARENT] shared->total_clients_served = %d", shared->total_clients_served);
  println( "[PARENT] shared->num_queued = %d", shared->num_queued);
  println( "[PARENT] shared->data = %s", shared->data);

  
  if ( getpid() == parent_id ) { // clean up after all child processes have exited

    detachSharedMem( shared );
    destroySems();

    char shmid_str[ SMALL_BUFFER ];
    toString( shmid_str, shmid );
    cleanup( shmid_str );

  }

  return 0;
 
}
