
/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

/*

int getMostFreqItem();
void topItems();
int freq( int );
int clientWaitTime( int );
float avgWaitTime();
void printStats();
float avgWaitTime();
float itemRevenue( int );

*/

int getMostFreqItem() {

	int i, f, max_freq = -1, most_freq_item_id = -1;
	for ( i=0; i < NUM_MENU_ITEMS; i++ ) {
		f = freq( i );
		if ( f > max_freq ) {
			max_freq = f;
			most_freq_item_id = i+1;
		}
	}

	return most_freq_item_id;

}

void topItems() {

	int i = 0, TOP_N = 5, item_id;
	int top_items[ TOP_N ];

	while ( i < TOP_N ) {
		// get the most freq item, nullify its value, repeat
		item_id = getMostFreqItem(); // returns item_id
		top_items[ i ] = item_id;
		shared->freq_menu_items[ item_id-1 ] = -1; // remove as candidate for max freq
		i++;
	}

	printf( "\n" );
	printf( "Top %d Most Popular Items (Greatest -> Least)", TOP_N );

	for ( i=0; i < TOP_N; i++ ) {
		printf( "\n" );
		printf( "%d", top_items[ i ] );
		printf( " (%s)", getDescription( top_items[i] ) );
	}
	
	printf( "\n" );
	printf( "\n" );

}

int freq( int item_id ) {
	return shared->freq_menu_items[ item_id ];
}


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
	int f = freq( item_id-1 );
	return f * getPrice( item_id );
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

	printf( "\n" );
	printf( "\n" );
	topItems();

}
