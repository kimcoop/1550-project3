/*

char* toString( char*, int );
void signalHandler( int );
int installSignalHandler();
void writeToFile( char*, char* );
void emptyFile( char* );

*/



char* toString( char* str, int i ) {
  sprintf( str, "%d", i );
  return str;
}

void signalHandler( int sig ) {

  signal( sig, SIG_IGN );
  char c;
  printf( "\n\nDo you really want to quit? (y/n): ");
  c = getchar();
  if ( c == 'y' || c == 'Y' ) {
  	println( "Goodbye!" );
    println( "" );
    OPERATE = FALSE; // global var to close loops for paner, server, cashiers
  } else {
  	signal( SIGINT, signalHandler );
  }
}

int installSignalHandler() {
	if ( signal( SIGINT, signalHandler ) == SIG_ERR ) { // install signal handler
    println( "An error occurred while setting a signal handler." );
    return EXIT_FAILURE;
  }
}

void writeToFile( char* filename, char* str ) {
  FILE *file = fopen( filename, "ab+" ); // append file (add text to a file or create a file if it does not exist)
  fprintf( file, "%s ", str ); // write
  fprintf( file, "\n" );
  fclose( file );
}

void emptyFile( char* filename ) {
  log( "Clearing file contents from %s", filename );
  FILE *fp = fopen( filename, "w" ); // clear out contents
  fprintf( fp, "" );
  fclose( fp );
}