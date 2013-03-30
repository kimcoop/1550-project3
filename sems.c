/*

	int allocateSharedMem( key_t );
	SharedData* attachSharedMem( int );
	void detachSharedMem( SharedData* );
	void removeSharedMem( int );

  void p_sem_wait( sem_t* );
  void p_sem_post( sem_t* );
  void p_sem_open( sem_t*, int, char* );
  void p_sem_close( sem_t*, char* );
  void initSems();
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
	
	if ( shmctl( shmid, IPC_RMID, NULL ) == -1 ) {
    perror("shmtcl");
  }

}

/*
  SEMAPHORES
*/

void p_sem_wait( sem_t* sem ) {

  if (sem_wait( sem ) == -1) {
    perror( "sem_wait ");
  }

}

void p_sem_post( sem_t* sem ) {

  if (sem_post( sem ) == -1) {
    perror( "sem_post ");
  }

}

void p_sem_init( sem_t* sem, int flag1, int flag2 ) {

  if (sem_init( sem, flag1, flag2 ) == -1) {
    perror( "sem_init ");
  }

}

void p_sem_open( sem_t *sem, int flag, char* name ) {
  if (( sem = sem_open( name, O_CREAT, 0644, flag )) == SEM_FAILED ) {
    perror( "sem_open" );
  }
}

void p_sem_close( sem_t *sem, char* name ) {
  if ( sem_close( sem ) == -1 ) {
    perror( "sem_close ");
  }

  if ( sem_unlink( name  ) == -1 ) {
    perror( "sem_unlink ");
  }

}

void initSems() {
  // setting middle param to 0 means first process to sem will be blocked

  p_sem_open( &shared->waiting_queue_mutex, 1, "/waiting_queue_mutex" );
  p_sem_open( &shared->order_queue_mutex, 1, "/order_queue_mutex" );
  p_sem_open( &shared->db_mutex, 1, "/db_mutex" );
  p_sem_open( &shared->client_exit_mutex, 1, "/client_exit_mutex" );
  p_sem_open( &shared->menu_items_mutex, 1, "/menu_items_mutex" );
  p_sem_open( &shared->orders_mutex, 1, "/orders_mutex" );

  p_sem_open( &shared->cashier, 3, "/cashier" );
  p_sem_open( &shared->payment, 0, "/payment" );
  p_sem_open( &shared->receipt, 0, "/receipt" );
  p_sem_open( &shared->new_order, 0, "/new_order" );

  p_sem_open( &shared->ordered, 0, "/ordered" );
  p_sem_open( &shared->client_present, 0, "/client_present" );

  int i;
  for ( i=0; i< MAX_NUM_CLIENTS; i++ ) {
    char sem_name[ SMALL_BUFFER ];
    toString( sem_name, i );
    p_sem_open( &shared->signal_client[i], 0, sem_name );
  }

}

void destroySems() {

  println( "Destroying sempahores." );
  p_sem_close( &shared->menu_items_mutex, "/menu_items_mutex" );
  p_sem_close( &shared->db_mutex, "/db_mutex" );
  p_sem_close( &shared->orders_mutex, "/orders_mutex" );
  p_sem_close( &shared->waiting_queue_mutex, "/waiting_queue_mutex" );
  p_sem_close( &shared->order_queue_mutex, "/order_queue_mutex" );
  p_sem_close( &shared->client_exit_mutex, "/client_exit_mutex" );
  
  p_sem_close( &shared->payment, "/payment" );
  p_sem_close( &shared->new_order, "/new_order" );
  
  p_sem_close( &shared->receipt, "/receipt" );
  p_sem_close( &shared->cashier, "/cashier" );
  p_sem_close( &shared->ordered, "/ordered" );
  p_sem_close( &shared->client_present, "/client_present" );
  
  int i;
  for ( i=0; i< MAX_NUM_CLIENTS; i++ ) {
    char sem_name[ SMALL_BUFFER ];
    toString( sem_name, i );
    p_sem_close( &shared->signal_client[i], sem_name );
  }
}