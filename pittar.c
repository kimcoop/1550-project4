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

int FIRST_LEVEL = 0;
char CURR_DIR[ SMALL_BUFFER ];
Archive archive;
FILE* disk;

void write_to_disk(long insert_point, int f_size, char* name );

void init_archive() {
  
  // init respective pieces of Archive 
  archive.header = ( Header* ) malloc( sizeof( Header ) );
  archive.header = ( Header* ) malloc( sizeof( Header ) );
  int i;
  for ( i=0; i < MAX_FILES_IN_DIR; i++ )
    archive.meta_data[ i ] = ( MetaData* ) malloc( sizeof( MetaData ));

  strcpy( archive.header->d_name, DIR_NAME );
  archive.header->num_files = 0;

  disk = fopen( DISK_NAME, "a+b" );
  assert( disk != NULL );
}

void append_to_archive( char *name ) {

  // TODO; check overflow
  MetaData* data = ( MetaData* ) malloc( sizeof(MetaData) );
  strcpy( data->f_name, name );
  
  println(" archive data begins at %lu for file %s", (long) archive.data_block, name );

  int insert_index = archive.header->num_files;
  long insert_point = (long) archive.data_block; // assume 0 files (overwrite if more)

  println("insert point is %lu ", insert_point );

  if ( archive.header->num_files > 0 ) {
    int prev_index = archive.header->num_files - 1;
    MetaData* prev = archive.meta_data[ prev_index ];
    insert_point = prev->start + prev->f_size;
    println(" calculating data_block insertion point based on previous files in archive" );
  }

  println("inserting at index %d", insert_index);
  int f_size = get_file_size( name );

  data->f_size = f_size;
  data->start = insert_point;
  archive.meta_data[ archive.header->num_files ] =  data;

  // archive.data_block[ insert_point ] = get_contents( name );
  write_to_disk( insert_point, f_size, name );
  ++archive.header->num_files;
  println( "archive num_files increased to %d ", archive.header->num_files );

}

void write_to_disk( long insert_point, int f_size, char* name ) {

  println(" write_to_disk ");

  // FILE *fp;
// fp=fopen("c:\\test.bin", "wb");
// char x[10]="ABCDEFGHIJ";
// fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp);

  char* contents = malloc( f_size );
  FILE* fp = fopen( name, "rb" );
  fread( contents, 1, f_size, fp );
  println(" file %s contents:: ", name);
  println("------------------------------------------------");
  println(" %s", contents );
  println("------------------------------------------------");

  // pwrite( disk_descriptor, contents, f_size, 0 );
  fwrite( contents, sizeof( contents[0] ), f_size/sizeof( contents[0] ), disk );
  // pwrite( int fd, const void* buf, size_t count, off_t offset );

  fclose( fp );

}

void read_from_disk() {

  fseek( disk, 0, SEEK_SET);

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
  println( "d_name: %s", archive.header->d_name );
  println( "num_files: %d ", archive.header->num_files );

  int i;
  for ( i=0; i < archive.header->num_files; i++ ) {
    println( "meta_data file name: %s", archive.meta_data[ i ]->f_name );
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
    
    char flag;
    strcpy( CURR_DIR, "." );

    int i;
    for ( i=1; i < argc; i++ ) {
      
      flag = argv[i][1];
        // num_cashiers = atoi( argv[ ++i ] );
      if ( flag == 'p' )
        print_hierarchy( CURR_DIR );
      if ( flag == 'm' )
        print_meta_data( CURR_DIR );
      if ( flag == 'c' ) {
        init_archive();

        println(" iterating through files/directores in argv" );
        int j;
        for ( j=i+1; j < argc; j++ ) { // iterate through list of files/directories present in argv
          compress_file( argv[j] );          
        }

        append_to_archive( TEST_FILE );
        print_archive();
        read_from_disk();
        fclose( disk );
      }
      if ( flag == 'd' ) { // test decompress
        int j;
        for ( j=i+1; j < argc; j++ ) {
         decompress_file( argv[j] );
        }
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
