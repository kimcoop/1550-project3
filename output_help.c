/*
	void printClientOptions();
	void printCashierOptions();
	void printServerOptions();
*/

void printClientOptions() {
	println( "" );
	println(  "Client Options:" );
	println(  " -i <int>\t menu item ID " );
	println(  " -e <int> \t max time client spends eating her food" );
	println(  " -m <int>\t max people in queue" );
	println(  " -p <int>\t probably with which client decides to leave" );
	println(  " -h <int> \t shared memory segment ID" );
	println(  " -x <int> \t client ID" );
	println( "" );	
}

void printCashierOptions() {
	println( "" );
  println( "Cashier Options:" );
  println( " -s <int>\t max time cashier takes to service client" );
  println( " -b <int> \t max time cashier spends in break" );
  println( " -k <int> \t number of cashiers" );
  println( " -h <int> \t shared memory segment ID" );
  println( "" );
}

void printServerOptions() {
	println( "" );
  println( "Server Options:" );
  println( " -t <int>\t max time server takes to service client" );
  println( " -h <int> \t shared memory segment ID" );
  println( "" );
}

void printValues( int item_id, int eat_time, int max_people, int prob, int service_time, int break_time) {
  println( "Client Values:" );
  println( "-------");
  println( "menu ID of meal: %d", item_id );
  println( "max time client spends eating her food: %d", eat_time );
  println( "max people in queue: %d", max_people );
  println( "probably with which client decides to leave: %d", prob );
  println( "" );

  println( "Server/Cashier Values:" );
  println( "-------");
  println( "max time to service client: %i", service_time );
  println( "max time cashier spends in break: %i", break_time );
  println( "" );

}

