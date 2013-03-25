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
  if ( ( shmid = shmget( key, sizeof( SharedData ), 0644 | IPC_CREAT )) == -1 ) { 
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

	println("detachSharedMem");

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
  
  sem_init( &shared->waiting_queue_ready, 0, 0 );
  sem_init( &shared->mutex, 0, 1 );

}

void destroySems() {

	println("destroySems");
  sem_destroy( &shared->waiting_queue_ready );
	sem_destroy( &shared->mutex );

}