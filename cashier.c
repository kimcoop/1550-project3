/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013
*/

/*
void printValues();
void takeClient();
void takeBreak();
*/

#include  "my_header.h"

int service_time = SERVICE_TIME, 
    break_time = BREAK_TIME, 
    shared_id = SHARED_ID,
    cashier_id;

void printValues() {
  println("----");
  println( "max time cashier takes to service client: %i", service_time );
  println( "max time cashier spends in break: %i", break_time );
  println( "shared memory segment ID: %i", shared_id );
  println("----");
  println("");
}

  // char str[ SMALL_BUFFER ] = "Child ID is ";
  // strncat( str, "test", SMALL_BUFFER );
  // strncpy( shared->data, "test", SMALL_BUFFER );
  // println( "shared->data = %s", shared->data);

  // writeToFile( OUTPUTFILE, str );

void takeClient() {
  println("(CASHIER) taking client ");
  sem_post( &shared->cashier_ready );

}

void takeBreak() {
  println("(CASHIER) taking break ");
  sleep( break_time );
}

int main( int argc, char *argv[] ) {

  
  if ( argc == 1 ) { 
    printCashierOptions();
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
      else if ( strEqual(flag, "-b") ) 
        break_time = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-h") ) 
        shared_id = atoi( argv[ ++i ] );
      else if ( strEqual(flag, "-u") ) 
        cashier_id = atoi( argv[ ++i ] );
    }
  }

  
  // printValues();
  shared = attachSharedMem( shared_id );
  int i = 0;
  do {
    takeClient();
    takeBreak();
    i++;
  } while ( i < MAX_NUM_CLIENTS && OPERATE );

  println("( CASHIER )  detachSharedMem " );
  detachSharedMem( shared );


  return 0;
 
}
