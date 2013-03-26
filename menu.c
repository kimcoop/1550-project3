/*

 int getWaitTime( int );
 char* getDescription( int );
 float getPrice( int );
 int getMinTime( int );
 int getMaxTime( int );
 int isValidMenuOption( int );


*/


char* DESCRIPTIONS[] = { "BBQ-Chicken-Salad", "Spinach-Powder", "Garden-Salad", "Steak-Blue-Cheese", "Slovenian-Salad", "Hong-Kong-Chicken-Salad", "Mongolian-BBQ-Plate", "Club-Sandwich", "Belgian-Cheese-Sub", "Chipotle-Beef-Sub", "Argentine-Asado-Club", "Sierra-Turkey-Sub", "Avocado-BLT", "Soup-de-Egion", "Soup-de-Sur", "Coffee", "Hot-Tea", "Hot-Chocolate", "Mocha", "Cafe-Late" };
float PRICES[] = { 8.95, 9.15, 4.75, 7.25, 6.75, 9.15, 9.75, 6.35, 10.25, 9.35, 11.75, 10.38, 8.05, 3.20, 2.75, 1.25, 1.05, 2.15, 3.25, 3.75 };
int MIN_TIMES[] = { 180, 120, 100, 220, 130, 150, 210, 135, 150, 180, 230, 120, 120, 30, 20, 10, 10, 10, 25, 25 };
int MAX_TIMES[] = { 240, 160, 130, 240, 150, 180, 220, 185, 165, 200, 260, 150, 135, 35, 30, 20, 15, 25, 35, 40 };


int getWaitTime( int item_id ) {
  int max, min;
  max = getMaxTime( item_id );
  min = getMinTime( item_id );
  // return ( rand() % t );
  return max-min;
}

char* getDescription( int item_id ) {
  if ( isValidMenuOption( item_id ) ) {
    char* description = DESCRIPTIONS[ item_id-1 ];
    return description;
  } else {
    println( "%d is not a valid menu item. ", item_id );
    return NULL;
  }
}


float getPrice( int item_id ) {
  if ( isValidMenuOption( item_id ) ) {
    return PRICES[ item_id-1 ];
  } else {
    println( "%d is not a valid menu item. ", item_id );
    return -1;
  }
}


int getMinTime( int item_id ) {
  if ( isValidMenuOption( item_id ) ) {
    return MIN_TIMES[ item_id-1 ];
  } else {
    println( "%d is not a valid menu item. ", item_id );
    return -1;
  }
}


int getMaxTime( int item_id ) {
  if ( isValidMenuOption( item_id ) ) {
    return MAX_TIMES[ item_id-1 ];
  } else {
    println( "%d is not a valid menu item. ", item_id );
    return -1;
  }
}

int isValidMenuOption( int item_id ) {
  return item_id > 0 && item_id <= 20;
}
