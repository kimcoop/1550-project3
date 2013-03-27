
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

	printf( "\n" );
	printf( "ItemID \t | \tTimes Ordered \t| \tRevenue \t| \tDescription \t\n" );
	printf( "----------------\n" );
	printf( "\n" );
	int i, freq;
	float revenue;
	for ( i=1; i <= NUM_MENU_ITEMS; i++ ) {
		revenue = itemRevenue( i );
		freq = shared->freq_menu_items[ i-1 ];
		printf( "%d \t  ", i); // item_id
		printf( "%d \t  ", freq); // times ordered
		printf( "$%.2f \t  ", revenue); // revenue for item
		printf( "%s \t  ", getDescription( i ) ); // description
		printf( "\n" );
	}
	printf( "\n" );
	printf( "-------\n");
	printf( "Total revenue: $%.2f\n", totalRevenue() );
	printf( "Total clients served: %d\n", totalClients() );
	printf( "-------\n");

}





