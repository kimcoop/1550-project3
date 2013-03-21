/*

	int allocateSharedMem( key_t );
	char* attachSharedMem( int );
	void detachSharedMem( char* );
	void removeSharedMem( int );

	void initSems();
	void destroySemaphore();

*/

int allocateSharedMem( key_t key ) {
	// connect, maybe create, segment
	int shmid;
  if ( ( shmid = shmget( key, SHM_SIZE, 0644 | IPC_CREAT )) == -1 ) { 
    perror("shmget");
    exit(1);
  } else {
  	return shmid;
  }

}

char* attachSharedMem( int shmid ) {

	char* data;
  if ( (data = shmat( shmid, ( void* )0, 0 )) == ( char* )(-1) ) { //attach to get ptr
    exit(1);
    perror("shmat");
  } else {
  	return data;
  }

}

void detachSharedMem( char* data ) {

	println("detachSharedMem with data %s", data);

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
  
  sem_init( &shared.full, 0, 0 );
  sem_init( &shared.empty, 0, 0 );
  sem_init( &shared.mutex, 0, 1 );

}

void destroySemaphore() {
	println("destroySemaphore");
	sem_destroy( &shared.full );
  sem_destroy( &shared.empty );
  sem_destroy( &shared.mutex );
}