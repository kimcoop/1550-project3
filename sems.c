/*

	int allocateSharedMem( key_t );
	SharedData* attachSharedMem( int );
	void detachSharedMem( SharedData* );
	void removeSharedMem( int );

  
  void initSems();
  void destroySems();


*/

int allocateSharedMem( key_t key ) {
	// connect, maybe create, segment
	int shmid;

  //TODO use key
  if ( ( shmid = shmget( IPC_PRIVATE, sizeof( SharedData ), IPC_CREAT | 0660 )) == -1 ) { 
    perror("shmget");
    return -1;
  } else {
    return shmid;
  }

}

SharedData* attachSharedMem( int shmid ) {

	SharedData* data;
  if ( (data = shmat( shmid, ( void* )0, 0 )) == ( SharedData* )(-1) ) { //attach to get ptr
    return NULL;
    perror("shmat");
  } else {
  	return data;
  }

}

void detachSharedMem( SharedData* data ) {

  if ( shmdt(data) == -1 ) {
    perror("shmdt");
  }

}

void removeSharedMem( int shmid ) {
	
	if ( shmctl( shmid, IPC_RMID, NULL ) == -1 ) {
    perror("shmtcl");
  }

}

/*
  SEMAPHORES
*/

void initSems() {
  sem_init( &shared->waiting_queue_mutex, 0, 1 );
  sem_init( &shared->order_queue_mutex, 0, 1 );
  sem_init( &shared->new_order, 0, 1 );
  sem_init( &shared->client_exit_mutex, 0, 1 );
  sem_init( &shared->server_dispatch_ready, 0, 1 );
  sem_init( &shared->menu_items_mutex, 0, 1 );
  sem_init( &shared->db_mutex, 0, 1 );
  sem_init( &shared->orders_mutex, 0, 1 );
  ;

  int i;
  for ( i=0; i< MAX_NUM_CLIENTS; i++ ) {
    sem_init( &shared->signal_client[i], 0, 1 );
  }

}

void destroySems() {

  println( "Destroying sempahores." );
  sem_destroy( &shared->waiting_queue_mutex );
  sem_destroy( &shared->order_queue_mutex );
  sem_destroy( &shared->new_order );
  sem_destroy( &shared->client_exit_mutex );
  sem_destroy( &shared->server_dispatch_ready );
  sem_destroy( &shared->menu_items_mutex );
  sem_destroy( &shared->db_mutex );
  sem_destroy( &shared->orders_mutex );
  
  int i;
  for ( i=0; i< MAX_NUM_CLIENTS; i++ ) {
    sem_destroy( &shared->signal_client[i] );
  }

}