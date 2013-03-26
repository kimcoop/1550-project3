
/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 3
Due March 28, 2013

*/

/*

void printStats();


*/

void printStats() {

	println(" frequencies of items ordered:  ");
	int i;
	for ( i=0; i < NUM_MENU_ITEMS; i++ ) {
		printf( "%d: %d. ", i, shared->freq_menu_items[ i-1 ]);

	}
	println("");

}