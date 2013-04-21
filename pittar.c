/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 4
Due April 20, 2013
*/

#include "my_header.h"

#define DIR_NAME "coop"
#define DISK_NAME "archive.pitt"

void decompress_all();
void init_archive();
void read_from_disk();

int FIRST_LEVEL = 0;
char CURR_DIR[ SMALL_BUFFER ];
Archive archive;
FILE* disk;

void update_disk() {

  fseek( disk, 0, SEEK_SET );
  println(" update_disk: num_files %d ", archive.header.num_files );
  fwrite( &archive, sizeof( Archive ), 1, disk );

  Archive archive_test;
  fread( &archive_test, sizeof( Archive ), 1, disk );

}

void decompress_all() {

  int i;
  for ( i=0; i < archive.header.num_files; i++ ) {
    MetaData meta_data = archive.meta_data[i];

    char filename[ MAX_FILENAME ];
    strcpy( filename, meta_data.f_name );

    FILE* file_to_decompress = fopen( filename, "a+b" );
    println("should've created file with name %s", filename);

    char file_contents[ meta_data.f_size ];
    // strncpy( file_contents, archive.data_block[meta_data.f_start], meta_data.f_size );
    println(" retrived file_contents from archive.data_block ");

    // char *ext = ".Z";
    // char* new_name = (char *) malloc( strlen(filename) + strlen(ext)+1 );
    // new_name[0] = '\0';

    // strcpy( new_name, filename );
    // strcat( new_name, ext );

    println( "%s decompressing!!!", filename );

    decompress_file( filename );
  }
}

void init_archive() {
  
  if ( file_exists( DISK_NAME ) ) {

    println("getting existing archive");
    disk = fopen( DISK_NAME, "r+b" );

    fread( &archive, sizeof( Archive ), 1, disk );
    println(" READ IN*** header.d_name is %s ", archive.header.d_name );
    println(" NUM_FILES IS %d", archive.header.num_files );
    // println(" first filename is %s ", archive.meta_data[0].f_name );

    read_from_disk();

  } else {
    println(" disk does not exist yet; initializing " );

    strcpy( archive.header.d_name, DIR_NAME );
    archive.header.num_files = 0;
    archive.header.space_available = MAX_DATA_IN_BLOCK;
      
    disk = fopen( DISK_NAME, "a+b" );
    // /The arguments to fwrite() are the data to be printed, the size of one data item, the number of data items, and the file pointer.
    update_disk();
    println(" fwrote archive ");
  }
}



long get_insert_offset() {

  long insert_offset = (long) archive.data_block;
   
  if ( archive.header.num_files > 0 ) {
    int prev_index = archive.header.num_files - 1;
    MetaData prev = archive.meta_data[ prev_index ];
    insert_offset = prev.f_start + prev.f_size;
    println(" calculating data_block insertion point based on previous files in archive" );
  }

   println("insert offset is %lu ", insert_offset );

   return insert_offset;

}

void append_to_archive( char *name ) {

  // TODO: check overflow
  // TODO: check file named name not already present in archive 
  int f_size = get_file_size( name );
  if ( f_size >= archive.header.space_available ) {
    // println( "Error: not enough space for file %s in archive.", name );
    // return;
  } else {
    archive.header.space_available -= f_size;
    println(" remaining space availabe is %d", archive.header.space_available );
  }
  
  if ( strlen(name) > MAX_FILENAME )
    println( "Warning: filename %s is too long and will be truncated.", name );
  
  println(" archive data begins at %lu for file %s", (long) archive.data_block, name );

  long insert_offset = get_insert_offset();

  strcpy( archive.meta_data[ archive.header.num_files ].f_name, name );
  archive.meta_data[ archive.header.num_files ].f_start = insert_offset;
  archive.meta_data[ archive.header.num_files ].f_size = f_size;

  char* f_contents = malloc( f_size );
  FILE* fp = fopen( name, "rb" );

  fread( f_contents, 1, f_size, fp );
  strcat( archive.data_block, f_contents ); // concatenate the new file's contents to the archive's aggregate data_block

  ++archive.header.num_files;
  println( "archive num_files increased to %d ", archive.header.num_files );

  fclose( fp );
  println(" updating disk! num files is %d", archive.header.num_files );
  update_disk();

}

void read_from_disk() {

  // rewind to beginning of archive data block to read file contents
  // TODO: rewind to disk data block location
  fseek( disk, 0, SEEK_SET );

  char *buffer = malloc( BLOCK_SIZE );
  assert( buffer != NULL );
  int num_read;

  // fread(void *restrict ptr, size_t size, size_t nitems, FILE *restrict stream);

  while ( (num_read=fread( buffer, 1, BLOCK_SIZE, disk ))  > 0 ) {
    println("***buffer: %s", buffer );
    num_read = fread( buffer, 1, BLOCK_SIZE, disk );
    println("");
  }
  println(" read disk");
}


void print_archive() {

  println(" print_archive" );
  println( "d_name: %s", archive.header.d_name );
  println( "num_files: %d ", archive.header.num_files );

  int i;
  for ( i=0; i < archive.header.num_files; i++ ) {
    println("---FILE %d---", i );
    println( "meta_data file name: %s", archive.meta_data[ i ].f_name );
    println( "meta_data file size: %lu", archive.meta_data[ i ].f_size );
    println( "meta_data file start: %lu", archive.meta_data[ i ].f_start );
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
    
    char flag;
    strcpy( CURR_DIR, "." );
    init_archive();

    int i;
    for ( i=1; i < argc; i++ ) {
      
      flag = argv[i][1];
        // num_cashiers = atoi( argv[ ++i ] );
      if ( flag == 'p' ) {
        print_archive();
      }
      if ( flag == 'm' ) {
        print_archive_meta_data();
      }
      if ( flag == 'c' ) {

        println(" iterating through files/directores in argv" );
        int j;
        for ( j=i+1; j < argc; j++ ) { // iterate through list of files/directories present in argv
          char* compressed_file;
          compressed_file = compress_file( argv[j] );
          append_to_archive( compressed_file );
        }
      }
      if ( flag == 'x' ) { // unarchive (extract -> decompress)
        decompress_all();
      }
      if ( flag == 'k' ) {
        println(" iterating to decompress" );
        int j;
        for ( j=i+1; j < argc; j++ ) { // iterate through list of files/directories present in argv
          decompress_file( argv[j] );       
        }        
      }

    }
  }

  fclose( disk );
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
