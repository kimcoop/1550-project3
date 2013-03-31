
/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

/*

int clientWaitTime( int );
float avgWaitTime();
void printStats();
float avgWaitTime();
float itemRevenue( int );

*/


int clientWaitTime( int client_id ) {
	return shared->wait_times[ client_id ]; // 0-indexed
}

int totalWaitTime() {
	int i, sum = 0;
	for ( i=1; i <= MAX_NUM_CLIENTS; i++ ) {
		sum += clientWaitTime( i );
	}
	return sum;
}

float avgWaitTime() {
// from joining waiting_queue to leaving store

	int wait_time = totalWaitTime();
	if ( wait_time > 0 )
		return wait_time / shared->total_clients_served;
	else
		return 0.0;
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

void printStats() {

	printf( "\n" );
	printf( "  Item ID  | Times Ordered |  Revenue  |  Description\n" );
	printf( "-----------------------------------------------------------------------------------\n" );
	printf( "\n" );
	int i, freq;
	float revenue;
	for ( i=1; i <= NUM_MENU_ITEMS; i++ ) {
		revenue = itemRevenue( i );
		freq = shared->freq_menu_items[ i-1 ];
		printf( "\t%d \t  ", i ); // item_id
		printf( "%d \t  ", freq ); // times ordered
		printf( "$%.2f \t  ", revenue ); // revenue for item
		printf( "%s \t  ", getDescription( i ) ); // description
		printf( "\n" );
	}
	printf( "\n" );
	printf( "-------------------------\n" );
	printf( "Total revenue: $%.2f\n", totalRevenue() );
	printf( "Total clients served: %d\n", shared->total_clients_served );
	printf( "Average wait time: %.2f\n", avgWaitTime() );
	printf( "-------------------------\n" );

	printf( "\n" );
	printf( "  Client ID  | Shop Time |  Wait Time  |  $ Spent\n" );
	printf( "-----------------------------------------------------------------------------------\n" );
	printf( "\n" );
	for ( i = 0; i < shared->total_clients_served; i++ ) {
		// client_id, time spent in shop, time in waiting, money spent

		int time_in_shop = shared->time_in_shop[ i ];
		int time_waiting = shared->wait_times[ i ];
		int item_id = shared->orders[ i ];
		float money_spent = getPrice( item_id );

		printf( "\t%d ", i );
		printf( "\t%d ", time_in_shop );
		printf( "\t%d ", time_waiting );
		printf( "\t$%.2f ", money_spent );
		printf( "\n" );

	}

}
