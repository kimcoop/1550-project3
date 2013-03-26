/*
	void printClientOptions();
	void printCashierOptions();
	void printServerOptions();
*/

void printClientOptions() {
	println("");
	println("Client Options:");
	println( " -i <int>\t menu item ID ");
	println( " -e <int> \t max time client spends eating her food");
	println( " -m <int>\t max people in queue");
	println( " -p <int>\t probably with which client decides to leave");
	println( " -h <int> \t shared memory segment ID");
	println("");	
}

void printCashierOptions() {
	println("");
  println("Cashier Options:");
  println( " -s <int>\t max time cashier takes to service client" );
  println( " -b <int> \t max time cashier spends in break" );
  println( " -k <int> \t number of cashiers");
  println( " -h <int> \t shared memory segment ID");
  println("");
}

void printServerOptions() {
	println("");
  println("Server Options:");
  println( " -t <int>\t max time server takes to service client" );
  println( " -h <int> \t shared memory segment ID");
  println("");
}