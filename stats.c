
/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

/*

void printStats();
void clientStats();
void avgWaitTime();
int totalClients();
float itemRevenue( int );

*/

void clientStats() {
	// client_id, time spent in shop, time in waiting, money spent
}

void avgWaitTime() {
// from joining waiting_queue to leaving store
}

float itemRevenue( int item_id ) {
	int freq = shared->freq_menu_items[ item_id-1 ];
	return freq * getPrice( item_id );
}

float totalRevenue() {

	int i;
	float item_sum = 0, total_sum = 0;
	for ( i=1; i <= NUM_MENU_ITEMS; i++ ) {
		total_sum += itemRevenue( i );
	}
	return total_sum;
}

int totalClients() {
	return shared->total_clients_served;
}

void printStats() {

	println(" Frequencies of items ordered:  ");
	int i;
	for ( i=0; i < NUM_MENU_ITEMS; i++ ) {
		printf( "(%d: %d)  ", i, shared->freq_menu_items[ i-1 ]);
	}
	println( "" );
	println( "-------" );
	println( "Total revenue: %.2f", totalRevenue() );
	println( "Total clients served: %d", totalClients() );
	println( "-------" );

}





