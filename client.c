/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/


#include "my_header.h"

void shmemOps() {
  log("shmemOps");
  key_t key = ftok("kimcoop", 'R'); 
  int shmid = allocateSharedMem( key );
  char *data = attachSharedMem( shmid );

  log("shared contents: %s\n", data);
  
  pid_t child_pid;
  int i=0, j=0;
  if ( (child_pid = fork() ) < 0 ) {
    perror("fork"); exit(1);
  } else if ( child_pid == 0 ) {
    while (1 && i < 4) {
      // sem_wait( &shared.empty ); // if no empty slots, wait
      sem_wait( &shared.mutex ); // if another is using buffer, wait
      println(" child %d acquiring mutex ", getpid() );
      log(" (child) shared contents: %s\n", data);
      strncpy( data, "child! ", SHM_SIZE );
      sem_post( &shared.mutex );
      // sem_post( &shared.full );
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
  detachSharedMem( data );
  removeSharedMem( shmid );
}

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


  initSems();
  shmemOps();

  return 0;
 
}
