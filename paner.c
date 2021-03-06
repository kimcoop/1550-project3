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
void cashier( char* );
void server( char* );
void spawnClients( char* );
void initSharedData();
void initSharedMem();
*/

#include "my_header.h"

key_t key;
int shmid, num_clients, num_cashiers;
// client
int client_id, 
    item_id = ITEM_ID, 
    eat_time = EAT_TIME, 
    max_people = MAX_PEOPLE, 
    prob = PROBABILITY;

int service_time = SERVICE_TIME, break_time = BREAK_TIME; // server & cashier


void cleanup() {
  if ( execl( "./cleanup",  "cleanup", "all", (char*)0 ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void client( char* shmid_str, int client_id ) {
  char client_id_str[ SMALL_BUFFER ], item_id_str[ SMALL_BUFFER ], 
  eat_time_str[ SMALL_BUFFER ], max_people_str[ SMALL_BUFFER ], prob_str[ SMALL_BUFFER ];
  toString( client_id_str, client_id );
  toString( item_id_str, item_id );
  toString( eat_time_str, eat_time );
  toString( max_people_str, max_people );
  toString( prob_str, prob );

  char *const argv[] = {
    "client",
    "-h", shmid_str,
    "-x", client_id_str,
    "-i", item_id_str,
    "-e", eat_time_str,
    "-m", max_people_str,
    "-p", prob_str,
    (char*) 0
  };

  if ( execv( "./client", argv ) == -1 ) {
    perror( "execl" );
    exit( EXIT_FAILURE );
  }
}

void cashier( char* shmid_str ) {
  pid_t child_pid;
   if ( ( child_pid = fork() ) < 0 ) {
    perror("fork");
    exit( EXIT_FAILURE );
  } else if ( child_pid == 0 ) {
    if ( execl( "./cashier",  "cashier", "-h", shmid_str, (char*)0 ) == -1 ) {
      perror( "execl" );
      exit( EXIT_FAILURE );
    }
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

void spawnClients( char* shmid_str ) {
  
  pid_t child_pid;
  int i = 0;

  while ( i <= CLIENT_BATCH_SIZE && num_clients < MAX_NUM_CLIENTS ) {

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
  init_queue( &shared->waiting_queue );
  init_queue( &shared->order_queue );

  int i;
  for ( i=0; i < MAX_NUM_CLIENTS; i++ )
    shared->orders[ i ] = -1; // default each order to -1

  // set all mutexes to 1
  p_sem_post( &shared->waiting_queue_mutex );
  p_sem_post( &shared->order_queue_mutex );
  p_sem_post( &shared->db_mutex );
  p_sem_post( &shared->menu_items_mutex );
  p_sem_post( &shared->orders_mutex );
}

void initSharedMem() {
  key = ftok( KEY, KEY_MODE ); 
  shmid = allocateSharedMem( key );
  shared = attachSharedMem( shmid );
}

int main( int argc, char *argv[] ) {

  num_cashiers = NUM_CASHIERS;
  
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
      else if ( strEqual(flag, "-i") ) 
        item_id = atoi( argv[++i] );
      else if ( strEqual(flag, "-e") ) 
        eat_time = atoi( argv[++i] );  
      else if ( strEqual(flag, "-m") ) 
        max_people = atoi( argv[++i] );
      else if ( strEqual(flag, "-p") ) 
        prob = atoi( argv[++i] );
      else if ( strEqual(flag, "-s") ) 
        service_time = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-b") ) 
        break_time = atoi( argv[ ++i ] );
     
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

  printValues( item_id, eat_time, max_people, prob, service_time, break_time );

  char shmid_str[ SMALL_BUFFER ];
  toString( shmid_str, shmid );
  writeToFile( CLEANUP_FILE, shmid_str ); // track them in a file that we can parse with cleanup

  server( shmid_str );
  cashier( shmid_str );

  num_clients = 0; // helps assign client_id for each client spawned

  do { // produce clients in groups of CLIENT_BATCH_SIZE

    println( "..." );
    println( "%d clients approaching!", CLIENT_BATCH_SIZE );
    println( "..." );
    spawnClients( shmid_str );
    sleep( SLEEP_TIME );

  } while ( num_clients < MAX_NUM_CLIENTS );
  
  if ( getpid() == parent_id ) { // clean up after all child processes have exited

    printStats();
    detachSharedMem( shared ); // note that printStats() requires access to shared, so this must come after
    cleanup(); // scrapes shmids dumped in file and cleans them all

  }

  return 0;
 
}
