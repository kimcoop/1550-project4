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
-j archive the files contained in <file/directory list> in compressed form while creating <archive-firle>
*/

void print_contents( char* dirname ) {
  // println(" print_contents ");
  DIR   *dir_ptr;
  struct  dirent *direntp;
  struct stat mybuf;
  list( dirname, 0 );
}


int main( int argc, char *argv[] ) {
  
  if ( argc == 1 ) {
    // return 0;
  } else if ( argc == 2 ) { // --defaults, -d, etc
    println("Using defaults.");
  } else if ( (argc-1) % 2 != 0 ) {
    println("Malformed flags.");
    return EXIT_FAILURE;
  } else { // overwrite defaults
    
    int i;
    char flag[ SMALL_BUFFER ];

    for ( i = 1; i < argc; i++ ) {
      strcpy( flag, argv[ i ] );
      if ( strEqual(flag, "-k") )
        println(" todo ");
        // num_cashiers = atoi( argv[ ++i ] );
    }
  }


  char container[ SMALL_BUFFER ];
  strcpy( container, "." );
  print_contents( container );
  return 0;
 
}
