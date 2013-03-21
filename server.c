/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

#include  "my_header.h"

key_t key;
int shmid; 
char *data;

void sharedMemOps() {
  log("shmemOps");

  key = ftok("kimcoop", 'R'); 
  shmid = allocateSharedMem( key );
  data = attachSharedMem( shmid );
  
  pid_t child_pid;
  int i=0, j=0;
  if ( (child_pid = fork() ) < 0 ) {
    perror("fork"); 
    exit(1);
  } else if ( child_pid == 0 ) {
    char arg_shmid[SMALL_BUFFER];
    sprintf( arg_shmid, "%d", shmid );

    while (1 && i < 4) {
      
      if ( execl( "./client",  "client", "-h", arg_shmid, (char*)0 ) == -1 ) { // spawn clients
        perror("execl");
      }
      sem_wait( &shared.empty ); // if no empty slots, wait
      sem_wait( &shared.mutex ); // if another is using buffer, wait
      println(" child %d acquiring mutex ", getpid() );
      log(" (child) shared contents: %s\n", data);
      strncpy( data, "child! ", SHM_SIZE );
      sem_post( &shared.mutex );
      sem_post( &shared.full );
      i++;
    }
  } else { // parent
    while (1  && j < 4) {
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

  int service_time = SERVICE_TIME, shared_id = SHARED_ID;
  println("%s", argv[0]);
  if ( argc == 1 ) { 
    printServerOptions();
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
      else if ( strEqual(flag, "-h") ) 
        shared_id = atoi( argv[ ++i ] );
    }
  }

  println( "max time server takes to service client: %i", service_time );
  println( "shared memory segment ID: %i", shared_id );
  println("");

  int parent_id = getpid();
  initSems();
  sharedMemOps();
  println("waiting");
  wait( NULL ); // wait all child processes
  println("done waiting");
  if ( getpid() == parent_id ) {
    detachSharedMem( data );
    removeSharedMem( shmid );
  }
  return 0;
 
}
