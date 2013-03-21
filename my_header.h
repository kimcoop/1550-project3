#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define MEDIUM_BUFFER 32
#define SMALL_BUFFER 12
#define TRUE 1
#define FALSE 0
#define SHM_SIZE 1024

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
#define MAX_PEOPLE 5 // max people in queue for client to leave shop
#define PROBABILITY 50 // client decision to leave congested restaurant
#define SHARED_ID 1 // shared mem seg ID

/*
 GLOBALS
*/

typedef struct { 
 	int buf[ SMALL_BUFFER ];
 	int i;
 	sem_t full;
 	sem_t empty;
 	sem_t mutex; // enforce mutual exclusion to shared data
 } Semabuf;

 Semabuf shared;

 /* SEMS.C */
 	int allocateSharedMem( key_t );
	char* attachSharedMem( int );
	void detachSharedMem( char* );
	void removeSharedMem( int );

	void initSems();
	void destroySemaphore();

#include "sems.c"