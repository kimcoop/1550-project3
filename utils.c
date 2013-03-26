/*

char* toString( char*, int );
void signalHandler( int );
int installSignalHandler();
void writeToFile( char*, char* );

*/



char* toString( char* str, int i ) {
  sprintf( str, "%d", i );
  return str;
}

void signalHandler( int sig ) {
  puts("Interactive attention signal caught.");
  char c;
  signal( sig, SIG_IGN );
  printf( "OOF do you really want to quit? (y/n): ");
  c = getchar();
  if ( c == 'y' || c == 'Y' ) {
  	exit( EXIT_SUCCESS );
  	println( "Goodbye! ");
  } else {
  	signal( SIGINT, signalHandler );
  }
}

int installSignalHandler() {
	if ( signal( SIGINT, signalHandler ) == SIG_ERR ) { // install signal handler
    fputs( "An error occurred while setting a signal handler.\n", stderr );
    return EXIT_FAILURE;
  }
}

void writeToFile( char* filename, char* str ) {
  FILE *file = fopen( filename, "ab+" ); // append file (add text to a file or create a file if it does not exist)
  println(" writeToFile: %s", str );
  fprintf( file, "%s", str ); // write
  if ( DEBUG ) fprintf( file, "\n" );
  fclose( file );
}
