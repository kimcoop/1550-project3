/*

	int allocateSharedMem( key_t );
	SharedData* attachSharedMem( int );
	void detachSharedMem( SharedData* );
	void removeSharedMem( int );

	void initSems();
	void destroySems();


*/

allocateSharedMem( key_t key ) {
	// connect, maybe create, segment
	int shmid;

  //TODO use key
  if ( ( shmid = shmget( IPC_PRIVATE, sizeof( SharedData ), IPC_CREAT | 0660 )) == -1 ) { 
    perror("shmget");
    exit(1);
  } else {
   return shmid;
  }

}

SharedData* attachSharedMem( int shmid ) {

	SharedData* data;
  if ( (data = shmat( shmid, ( void* )0, 0 )) == ( SharedData* )(-1) ) { //attach to get ptr
    exit(1);
    perror("shmat");
  } else {
  	return data;
  }

}

void detachSharedMem( SharedData* data ) {

  if ( shmdt(data) == -1 ) {
    perror("shmdt");
    exit(1);
  }

}

void removeSharedMem( int shmid ) {
	println("removeSharedMem %d",shmid);
	if ( shmctl( shmid, IPC_RMID, NULL ) == -1 ) {
    perror("shmtcl");
    exit(1);
  }

}

//

void initSems() {
  
  sem_init( &shared->waiting_queue_mutex, 0, 1 );
  sem_init( &shared->order_queue_mutex, 0, 0 );
  sem_init( &shared->new_order, 0, 1 );
  sem_init( &shared->food_prepared, 0, 1 );
  sem_init( &shared->food_ready, 0, 1 );
  sem_init( &shared->serve_food, 0, 1 );
  sem_init( &shared->eating_food_mutex, 0, 1 );
  sem_init( &shared->client_exit_mutex, 0, 1 );

}

void destroySems() {

	println("destroySems");
  sem_destroy( &shared->waiting_queue_mutex );
  sem_destroy( &shared->order_queue_mutex );
  sem_destroy( &shared->new_order );
  sem_destroy( &shared->food_prepared );
  sem_destroy( &shared->food_ready );
  sem_destroy( &shared->eating_food_mutex );
  sem_destroy( &shared->client_exit_mutex );

}