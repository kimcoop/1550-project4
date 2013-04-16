/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 4
Due April 20, 2013
*/

#include "my_header.h"

int FIRST_LEVEL = 0;
char CURR_DIR[ SMALL_BUFFER ];
Archive archive;


void init_archive() {
  println("initArchive");
  strcpy( archive.d_name, "bambi" );
  archive.num_files = 0;
}

void insert_archive( char *name ) {
  println(" insert_archive file %s", name );
  println( "archive num_files is %d ", archive.num_files );

  // TODO; check overflow
  MetaData* data = ( MetaData* ) malloc( sizeof(MetaData) );
  strcpy( data->f_name, name );
  // TODO: extension
  
  println(" archive data begins at %lu ", (long)archive.data_block );

  int insert_index = archive.num_files;
  long insert_point = (long)archive.data_block;
  println("insert point is %lu ", insert_point );
  if ( archive.num_files > 0 ) {
    int prev_index = archive.num_files - 1;
    MetaData* prev = archive.meta_data[ prev_index ];
    insert_point = prev->start + prev->f_size;
  }

  println("inserting at index %d", insert_index);
  int f_size = get_file_size( name );

  data->f_size = f_size;
  data->start = insert_point;
  archive.meta_data[ archive.num_files ] =  data;

  // need to get the contents of the file

  archive.data_block[ insert_point ] = get_contents( name );
  ++archive.num_files;
  println( "archive num_files is %d ", archive.num_files );

}


void print_archive() {

  println(" print_archive" );
  println( "d_name: %s", archive.d_name );
  println( "num_files: %d ", archive.num_files );

  int i;
  for ( i=0; i < archive.num_files; i++ ) {
    println( "meta_data file name: %s", archive.meta_data[ i ]->f_name );
    // println( "meta_data file ext: %s", archive.meta_data[ i ]->f_name );
    println( "meta_data file size: %lu", archive.meta_data[ i ]->f_size );
    println( "meta_data file start: %lu", archive.meta_data[ i ]->start );
  }
}

void print_hierarchy( char* dirname ) {

  println("");
  println(" HIERARCHY FOR ARCHIVE FILE %s ", dirname );
  println("");

  hierarchy( dirname, FIRST_LEVEL );
}


void print_meta_data( char* dirname ) {

  println("");
  println(" META-DATA FOR ARCHIVE FILE %s ", dirname );
  println("");

  list_meta_data( dirname );
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
      if ( flag == 'c' ) {
        init_archive();
        insert_archive( "testfile.c" );
        print_archive();
        // ensure file list present
        // one at a time, append files from list into archive
      }

    }
  }

  return 0;
 
}

/*
-c <archive-file> <file/directory list>
-a append <file/directory list> in existing archive file <archive-file>
-x extract all files and catalogs from archived from <archive-file>
-m print out meta-data (owner, group, rights) from <archive-file>
-p display file hierarch(y/ies) readably (!) from files/directories stored in <archive-file>
-j archive the files contained in <file/directory list> in compressed form while creating <archive-file>
*/
