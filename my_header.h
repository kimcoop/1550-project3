#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>


#define MEDIUM_BUFFER 32
#define SMALL_BUFFER 12
#define TRUE 1
#define FALSE 0
#define KEY "/dev/null"
#define KEY_MODE 'R'
#define OUTPUT_FILE "output.txt"
#define DB_FILE "orders.txt"
#define CLEANUP_FILE "shmids.txt"
#define INITIAL_QUEUE_SIZE 10
#define QUEUE_SIZE 1000
#define SLEEP_TIME 20
#define CLIENT_BATCH_SIZE 1
#define MAX_NUM_CLIENTS 1
#define DB_PRINT_FORMAT "Client %d ordered item %d (%s, $%.2f)"



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
#define CLIENT_ID -1
#define EAT_TIME 5 // max time random [1..eat_time] client spends eating food before leaving
#define MAX_PEOPLE 5 // max people in queue for client to question entering shop
#define PROBABILITY 50 // client decision to leave congested restaurant
#define SHARED_ID 1 // shared mem seg ID
#define NUM_CASHIERS 1
#define NUM_MENU_ITEMS 20

/*
 GLOBALS
*/
int USE_DEFAULTS = FALSE;
int OPERATE = TRUE;

typedef struct {
  int q[QUEUE_SIZE+1];		// body of queue
  int first;              // position of first element
  int last;               // position of last element
  int count;              // number of queue elements
} Queue;

typedef struct { 
 	
 	int num_in_store, total_clients_served, num_queued, num_eating, num_exited;
 	int food_ready_client_id; // client_id for server to alert food is ready
 	int total_wait_time; // avg wait time (enter store -> leave)
 	
 	float total_revenue;
 	Queue waiting_queue; // clients first arrive here
 	Queue order_queue; // clients move here after placing order

 	int orders[MAX_NUM_CLIENTS]; // track client orders ( indexed by client_id )
 	int freq_menu_items[NUM_MENU_ITEMS]; // track top (5) most popular menu items & how much each has generated
 	sem_t signal_client[MAX_NUM_CLIENTS]; // signal cashier ready and food ready for client indexed by ID

 	sem_t waiting_queue_mutex, order_queue_mutex, orders_mutex;
 	sem_t cashier, ordered, payment, receipt;  // cashier interactions (placing order, paying, getting receipt)

 	sem_t new_order, meal_dispatch; // alert the server to new order

 	sem_t cashier_order_placed; // signal client that order was successful
 	sem_t server_mutex; // server is prepared to give client food
 	sem_t client_exit_mutex;
 	sem_t menu_items_mutex;
 	sem_t db_mutex; // dumping order to file

 } SharedData;

SharedData* shared;

/*
 PANER.C
*/
void cleanup();
void client( char*, int );
void cashier( char*, int );
void server( char* );
void spawnClients( char* );
void spawnCashiers( char* );
void initSharedData();
void initSharedMem();

/*
 CLIENT.C
*/
void arrive();
void waitForCashier();
void order();
void storeOrder();
void pay();
void getFood();
void eat();
void leave();
void printValues();

/* 
 UTILS.C 
*/
char* toString( char*, int );
void signalHandler( int );
int installSignalHandler();
void writeToFile( char*, char* );
void emptyFile( char* );

/* 
 SEMS.C 
*/
int 		allocateSharedMem( key_t );
SharedData*	  attachSharedMem( int );
void 		detachSharedMem( SharedData* );
void 		removeSharedMem( int );
void p_sem_wait( sem_t* );
void p_sem_post( sem_t* );
void 		initSems();
void 		destroySems();

/* 
 STATS.C
*/
 void printStats();

/* 
 OUTPUT_HELP.C 
*/
void	 printClientOptions();
void	 printCashierOptions();
void	 printServerOptions();

/*
 MENU.C
*/
int 		getWaitTime( int );
char* 	getDescription( int );
float 	getPrice( int );
int 		getMinTime( int );
int 		getMaxTime( int );
int 		isValidMenuOption( int );

/*
 HELPERS
*/
#include "utils.c"
#include "sems.c"
#include "queue.c"
#include "stats.c"
#include "output_help.c"
#include "menu.c"
