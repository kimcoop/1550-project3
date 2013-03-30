/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

/*
void cleanup();
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


void cleanup() {
  if ( execl( "./cleanup",  "cleanup", "all", (char*)0 ) == -1 ) {
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
  shared->num_exited = 0;
  shared->OPERATE = TRUE;
  init_queue( &shared->waiting_queue );
  init_queue( &shared->order_queue );

  int i;
  for ( i=0; i < MAX_NUM_CLIENTS; i++ )
    shared->orders[ i ] = -1; // default each order to -1

  // set all mutexes to 1
  p_sem_post( &shared->waiting_queue_mutex );
  p_sem_post( &shared->order_queue_mutex );
  p_sem_post( &shared->db_mutex );
  p_sem_post( &shared->server_mutex );
  p_sem_post( &shared->client_exit_mutex );
  p_sem_post( &shared->menu_items_mutex );
  p_sem_post( &shared->orders_mutex );
}

void initSharedMem() {
  key = ftok( KEY, KEY_MODE ); 
  println(" KEY******: 0x%x", key );
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

  // some housekeeping
  installSignalHandler();
  emptyFile( DB_FILE );
  int parent_id = getpid(); // gather while we know we are parent (only) process
  setbuf( stdout, NULL ); // stdout is unbuffered

  initSharedMem();
  initSems();
  initSharedData();

  char shmid_str[ SMALL_BUFFER ];
  toString( shmid_str, shmid );
  writeToFile( CLEANUP_FILE, shmid_str ); // track them in a file that we can parse with cleanup

  server( shmid_str );
  spawnCashiers( shmid_str );

  num_clients = 0; // helps assign client_id for each client spawned
  println( "PANER shared->OPERATE %d ", shared->OPERATE );

  do { // produce clients in groups of CLIENT_BATCH_SIZE
    spawnClients( shmid_str );
    sleep( SLEEP_TIME );
    if ( !shared->OPERATE ) break;
    println( "..." );
    println( "%d more clients approaching!", CLIENT_BATCH_SIZE );
    println( "..." );
  } while ( shared->OPERATE && num_clients < MAX_NUM_CLIENTS );
  
  if ( getpid() == parent_id ) { // clean up after all child processes have exited
    printStats();
    println( "Waiting for child processes to exit. " );
    sleep( 5 );
    detachSharedMem( shared ); // note that printStats() requires access to shared, so this must come after
    cleanup(); // scrapes shmids dumped in file and cleans them all
  }

  return 0;
 
}
