/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 4
Due April 20, 2013
*/

#include "my_header.h"

/*
-c <archive-file> <file/directory list>
-a append <file/directory list> in existing archive file <archive-file>
-x extract all files and catalogs from archived from <archive-file>
-m print out meta-data (owner, group, rights) from <archive-file>
-p display file hierarch(y/ies) readably (!) from files/directories stored in <archive-file>
-j archive the files contained in <file/directory list> in compressed form while creating <archive-file>
*/

int FIRST_LEVEL = 0;
char CURR_DIR[ SMALL_BUFFER ];


void print_hierarchy( char* dirname ) {

  println("");
  println(" HIERARCHY FOR ARCHIVE FILE %s ", dirname );
  println("");

  list( dirname, FIRST_LEVEL );
}


void print_meta_data( char* dirname ) {

  println("");
  println(" META-DATA FOR ARCHIVE FILE %s ", dirname );
  println("");

  // list_meta_data( dirname, FIRST_LEVEL );
}


int main( int argc, char *argv[] ) {
  

  if ( argc == 1 ) {
    println( "No operation specified. Exiting." );
    return 0;
  } else { // overwrite defaults
    
    int i;
    // char flag[ SMALL_BUFFER ];
    char flag;
    strcpy( CURR_DIR, "." );

    for ( i = 1; i < argc; i++ ) {
      
      flag = argv[i][1];
        // num_cashiers = atoi( argv[ ++i ] );
      if ( flag == 'p' )
        print_hierarchy( CURR_DIR );
      if ( flag == 'm' )
        print_meta_data( CURR_DIR );
    }
  }

  return 0;
 
}
