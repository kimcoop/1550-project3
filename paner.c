/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

/*
void endDay();
void cleanup( char* );
void client( char*, int );
void cashier( char*, int );
void server( char* );
void spawnCashiers( char* );
void spawnClients( char* );
void initSharedData();
void initSharedMem();
*/

#include "my_header.h"

key_t key;
int shmid, num_clients, num_cashiers;

void endDay() {
}

void cleanup( char* shmid_str ) {
  if ( execl( "./cleanup",  "cleanup", shmid_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void client( char* shmid_str, int client_id ) {
  char client_id_str[ SMALL_BUFFER ];
  toString( client_id_str, client_id );

  if ( execl( "./client",  "client", "-h", shmid_str, "-x", client_id_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void cashier( char* shmid_str, int cashier_id ) {
  char cashier_id_str[ SMALL_BUFFER ];
  toString( cashier_id_str, cashier_id );

  if ( execl( "./cashier",  "cashier", "-h", shmid_str, "-u", cashier_id_str, (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void server( char* shmid_str) {
  pid_t child_pid;
  if ( ( child_pid = fork() ) < 0 ) {
    perror("fork");
    exit( EXIT_FAILURE );
  } else if ( child_pid == 0 ) {
    if ( execl( "./server",  "server", "-h", shmid_str, (char*)0 ) == -1 ) {
      perror( "execl" );
      exit( EXIT_FAILURE );
    }
  }
}

void spawnCashiers( char* shmid_str ) {
  
  pid_t child_pid;
  int i = 0;

  while ( i < num_cashiers ) {

    if ( (child_pid = fork() ) < 0 ) {
      perror("fork"); 
      exit( EXIT_FAILURE );
    } else if ( child_pid == 0 ) {
      cashier( shmid_str, i );
    }

    i++;
  }
}
void spawnClients( char* shmid_str ) {
  
  pid_t child_pid;
  int i = 0;

  while ( i < CLIENT_BATCH_SIZE && num_clients < MAX_NUM_CLIENTS ) {

    if ( (child_pid = fork() ) < 0 ) {
      perror("fork"); 
      exit( EXIT_FAILURE );
    } else if ( child_pid == 0 ) {
      client( shmid_str, num_clients );
    }

    num_clients = num_clients++;
    i++;
  }
}

void initSharedData() {
  shared->total_clients_served = 0;
  shared->num_queued = 0;
  shared->total_revenue = 0.0;
  shared->total_wait_time = 0;
  shared->num_eating = 0;
  shared->num_exited = 0;
  shared->num_in_store = 0;
  init_queue( &shared->waiting_queue );
  init_queue( &shared->order_queue );
}

void initSharedMem() {
  key = ftok( KEY, KEY_MODE ); 
  shmid = allocateSharedMem( key );
  shared = attachSharedMem( shmid );
}

int main( int argc, char *argv[] ) {

  num_cashiers = NUM_CASHIERS; // TODO - ensure we pass other args into exec calls
  
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

  installSignalHandler();

  int parent_id = getpid(); // gather while we know we are parent (only) process
  setbuf( stdout, NULL ); // stdout is unbuffered
  
  openSems();
  initSharedMem();
  initSharedData();

  char shmid_str[ SMALL_BUFFER ];
  toString( shmid_str, shmid );
  writeToFile( CLEANUP_FILE, shmid_str ); // track them in a file that we can parse with cleanup

  println( "[ PARENT ] shared->total_clients_served = %d", shared->total_clients_served);
  println( "[ PARENT ] shared->num_queued = %d", shared->num_queued);

  server( shmid_str );
  spawnCashiers( shmid_str );

  num_clients = 0;
  while ( OPERATE ) { // produce clients in groups of CLIENT_BATCH_SIZE
    spawnClients( shmid_str );
    sleep( SLEEP_TIME );
    if ( !OPERATE ) break;
    println( "..." );
    println( "More clients approaching!" );
    println( "..." );
  }
  
  if ( getpid() == parent_id ) { // clean up after all child processes have exited
    println("[ PARENT ] detaching");
    printStats();
    detachSharedMem( shared );
  }

  return 0;
 
}
