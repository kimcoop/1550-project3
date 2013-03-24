#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>

#define MEDIUM_BUFFER 32
#define SMALL_BUFFER 12
#define TRUE 1
#define FALSE 0
#define SHM_SIZE 1024
#define KEY "kimcooperrider"
#define KEY_MODE 'R'
#define OUTPUTFILE "output.txt"

/* 
 DEBUGGING -  SET THIS VALUE TO 1 TO LOG OUTPUT
*/
#define DEBUG 1
#define println(...) if ( DEBUG ) { printf("%d:\t", __LINE__); }  printf( __VA_ARGS__ ); printf("\n")
#define log(...) if ( DEBUG ) { printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n"); }
#define strEqual(a, b) !strcmp(a, b)

/*
 DEFAULTS (overwritten by command line args)
 */
#define SERVICE_TIME 5 // max time random [1..service_time] this cashier takes to service
#define BREAK_TIME 5 // max break time random [1..break_time]
#define ITEM_ID 1 // menu ID of the meal
#define EAT_TIME 5 // max time random [1..eat_time] client spends eating food before leaving
#define MAX_PEOPLE 5 // max people in queue for client to question entering shop
#define PROBABILITY 50 // client decision to leave congested restaurant
#define SHARED_ID 1 // shared mem seg ID
#define NUM_CASHIERS 2

/*
 GLOBALS
*/
int USE_DEFAULTS = FALSE;

typedef struct { 
 	
 	int total_clients;
 	float total_revenue;
 	int total_wait_time; // avg wait time (enter store -> leave)
 	//TODO: top 5 most popular menu items & how much each has generated

 	char data[ SMALL_BUFFER ];
 	sem_t full;
 	sem_t empty;
 	sem_t mutex; // enforce mutual exclusion to shared data
 } SharedData;

SharedData* shared;




/* 
 SEMS.C 
*/
	int 		allocateSharedMem( key_t );
	SharedData*	  attachSharedMem( int );
	void 		detachSharedMem( SharedData* );
	void 		removeSharedMem( int );
	void 		initSems();
	void 		destroySemaphore();

/* 
 OUTPUT_HELP.C 
*/
	void	 printClientOptions();
	void	 printCashierOptions();
	void	 printServerOptions();

/*
 MENU.C
*/
	char* 	getDescription( int );
	float 	getPrice( int );
	int 		getMinTime( int );
	int 		getMaxTime( int );
	int 		isValidMenuOption( int );

/*
 HELPERS
*/
#include "sems.c"
#include "menu.c"
#include "output_help.c"



void writeToFile( char* filename, char* str ) {
  FILE *file = fopen( filename, "ab+" ); // append file (add text to a file or create a file if it does not exist)
  println(" writeToFile: %s", str );
  fprintf( file, "%s", str ); // write
  if ( DEBUG ) fprintf( file, "\n" );
  fclose( file );
}
