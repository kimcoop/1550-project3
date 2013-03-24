/*

char* toString( char*, int );

*/



char* toString( char* str, int i ) {
  sprintf( str, "%d", i );
  return str;
}