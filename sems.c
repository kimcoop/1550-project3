/*

	int allocateSharedMem( key_t );
	SharedData* attachSharedMem( int );
	void detachSharedMem( SharedData* );
	void removeSharedMem( int );

  void initSem( sem_t*, char* );
  void initSems();
  void closeSem( sem_t*, char* );
  void destroySems();


*/

int allocateSharedMem( key_t key ) {
	// connect, maybe create, segment
	int shmid;

  //TODO use key
  if ( ( shmid = shmget( key, sizeof( SharedData ), IPC_CREAT | 0660 )) == -1 ) { 
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
	println("removeSharedMem %d",shmid);
	if ( shmctl( shmid, IPC_RMID, NULL ) == -1 ) {
    perror("shmtcl");
  }

}

/*
  SEMAPHORES
*/

void initSem( sem_t *semaphore, char* name ) {
  if (( semaphore = sem_open( name, O_CREAT, 0644, 1 )) == SEM_FAILED ) {
    perror( "sem_open" );
    exit( EXIT_FAILURE );
  }
}

void initSems() {
  
  initSem( &shared->waiting_queue_mutex, "/waiting_queue_mutex" );
  initSem( &shared->order_queue_mutex, "/order_queue_mutex" );
  initSem( &shared->new_order, "/new_order" );
  initSem( &shared->client_exit_mutex, "/client_exit_mutex" );
  initSem( &shared->server_dispatch_ready, "/server_dispatch_ready" );
  initSem( &shared->cashier_ready, "/cashier_ready" );

  int i;
  for ( i=0; i< MAX_NUM_CLIENTS; i++ ) {
    char sem_name[ SMALL_BUFFER ];
    toString( sem_name, i );
    initSem( &shared->order_up[i], sem_name );
  }

}

void closeSem( sem_t *semaphore, char* name ) {

  if ( sem_unlink( name  ) == -1 ) {
    perror( "sem_unlink ");
    exit( EXIT_FAILURE );
  }
  if ( sem_close( semaphore ) == -1 ) {
    perror( "sem_close ");
    exit( EXIT_FAILURE );
  }

}

void destroySems() {

  closeSem( &shared->waiting_queue_mutex, "/waiting_queue_mutex" );
  closeSem( &shared->order_queue_mutex, "/order_queue_mutex" );
  closeSem( &shared->new_order, "/new_order" );
  closeSem( &shared->client_exit_mutex, "/client_exit_mutex" );
  closeSem( &shared->server_dispatch_ready, "/server_dispatch_ready" );
  closeSem( &shared->cashier_ready, "/cashier_ready" );

  int i;
  for ( i=0; i< MAX_NUM_CLIENTS; i++ ) {
    char sem_name[ SMALL_BUFFER ];
    toString( sem_name, i );
    closeSem( &shared->order_up[i], sem_name );
  }

}