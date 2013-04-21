/*
Kim Cooperrider
KAC162@pitt.edu
CS1550
Project 4
Due April 20, 2013
*/

#include "my_header.h"

#define DIR_NAME "(no dir)"
#define DISK_NAME "archive.pitt"

void decompress_all();
void init_archive();
void read_files();


void update_disk() {

  println("here5");
  assert( disk != NULL );

  fseek( disk, 0L, SEEK_SET );

  println("here6");
  fwrite( &archive, sizeof( Archive ), 1, disk );
  println("here7");

  fseek( disk, 0L, SEEK_END );
  println("Updated disk (%d files)", archive.header.num_files );

}

void decompress_all() {

  if ( archive.header.num_files == 0 ) {
    println( "Error: no files present in archive to extract" );
    return;
  }

  int i;
  for ( i=0; i < archive.header.num_files; i++ ) {
    MetaData meta_data = archive.meta_data[i];

    char filename[ MAX_FILENAME + 1];
    strcpy( filename, "m" );
    strcat( filename, meta_data.f_name );

    FILE* file_to_decompress = fopen( filename, "wb" );
    println( "Reconstructing file %s", filename );

    char file_contents[ meta_data.f_size ];
    strcpy( file_contents, archive.data_block+meta_data.f_start );
    fwrite( &file_contents, meta_data.f_size, 1, file_to_decompress );
    // TODO: remove from archive

    decompress_file( filename );
  }
}

void init_archive() {
  
  if ( file_exists( DISK_NAME ) ) {

    println( "Retrieving existing archive..." );
    disk = fopen( DISK_NAME, "rb+" );

    fread( &archive, sizeof( Archive ), 1, disk );
    println( "Retrieved archive containing directory named %s with %d files", archive.header.d_name, archive.header.num_files );

  } else {
    println( "Initializing archive" );

    strcpy( archive.header.d_name, DIR_NAME );
    archive.header.num_files = 0;
    archive.header.space_available = MAX_DATA_IN_BLOCK;
      
    disk = fopen( DISK_NAME, "wb+" );
    update_disk();

  }
}


long get_insert_offset( char *name ) {
  // return offset for file name in archive

  long insert_offset = 0; // assume first file in archive (will be at archive.data_block+0)
  
  int i;
  for ( i=0; i <= archive.header.num_files; i++ ) {
    insert_offset += archive.meta_data[ i ].f_size;
  }

   return insert_offset;

}

void append_to_archive( char *name ) {

  int i;
  for ( i=0; i < archive.header.num_files; i++ ) {
    if ( strEqual( archive.meta_data[ i ].f_name, name ) ) {
      println( "Error: file %s is already present in archive", name );
      return;
    }
  }

  int f_size = get_file_size( name );
  if ( f_size >= archive.header.space_available ) { // TODO: check overflow
    // println( "Error: not enough space for file %s in archive.", name );
    // return;
  } else {
    archive.header.space_available -= f_size;
  }
  // if ( sizeof(name) > MAX_FILENAME )
  //   println( "Warning: filename %s is too long (will be truncated)", name );

  int modes = get_perms( name );
  long insert_offset = get_insert_offset( name );

  strcpy( archive.meta_data[ archive.header.num_files ].f_name, name );
  archive.meta_data[ archive.header.num_files ].f_start = insert_offset;
  archive.meta_data[ archive.header.num_files ].f_size = f_size;
  archive.meta_data[ archive.header.num_files ].f_modes = modes;

  char* f_contents = malloc( f_size );
  FILE* fp = fopen( name, "rb" );

  fread( f_contents, f_size, 1, fp );
  strcpy( archive.data_block+insert_offset, f_contents );
  
  archive.header.num_files += 1;

  fclose( fp );
  free( f_contents );
  update_disk();

}

void read_files() {

  // rewind to beginning of archive data block to read file contents
  fseek( disk, 0L, SEEK_SET );
  println("");

  int i;
  for ( i=0; i < archive.header.num_files; i++ ) {
    println("---FILE %s---", archive.meta_data[ i ].f_name );
    int f_size = archive.meta_data[ i ].f_size;
    char* f_contents = malloc( f_size );
    long offset = sizeof(Header) + archive.meta_data[i].f_start;
    fseek( disk, offset, SEEK_SET );
    fread( f_contents, f_size, 1, disk );
    println("%s", f_contents);
  }
}

void _print_hierarchy( char* dirname ) {

  println("");
  println(" HIERARCHY FOR DIRECTORY %s ", dirname );
  println("");

  hierarchy( dirname, FIRST_LEVEL );
}


void _print_meta_data( char* dirname ) {

  println("");
  println(" META-DATA FOR DIRECTORY %s ", dirname );
  println("");

  list_meta_data( dirname );
}


int main( int argc, char *argv[] ) {
  

  if ( argc == 1 ) {
    println( "No operation specified. Exiting" );
    return 0;
  } else { // overwrite defaults
    
    char flag;
    strcpy( CURR_DIR, "." );

    int COMPRESS_FLAG = FALSE;

    int i;
    for ( i=1; i < argc; i++ ) {
      
      flag = argv[i][1];
      if ( flag == 'f' ) {
        char* str = ""
        "\t -c \t <archive-file> <file/directory list>\n"
        "\t -k \t <file list> decompress files in working directory\n"
        "\t -h \t print hierarchy for current directory\n"
        "\t -y \t print meta data for files in current directory\n"
        "\t -a \t append <file/directory list> in existing archive file <archive-file>\n"
        "\t -x \t extract all files and catalogs from archived from <archive-file>\n"
        "\t -m \t print out meta-data (owner, group, rights) from <archive-file>\n"
        "\t -p \t display file hierarch(y/ies) readably from files/directories stored in <archive-file>\n"
        "\t -j \t archive the files contained in <file/directory list> in compressed form while creating <archive-file>\n";

        println( "--OPTIONS--" );
        println( "%s", str );
      }
      if ( flag == 'h' )
        _print_hierarchy( CURR_DIR );
      if ( flag == 'y' )
        _print_meta_data( CURR_DIR );
      if ( flag == 'j' ) {
        COMPRESS_FLAG = TRUE;
      } 
      if ( flag == 'p' ) {
        init_archive();
        print_archive();
      }
      if ( flag == 'm' ) {
        init_archive();
        print_archive_meta_data();
      }
      if ( flag == 'a' ) {
        init_archive();
        if ( COMPRESS_FLAG ) { println( "Compressing files" ); }
        if ( !COMPRESS_FLAG ) { println( "Not compressing files" ); }
        assert( COMPRESS_FLAG );
        int j;
        for ( j=i+1; j < argc; j++ ) { // iterate through list of files/directories present in argv
          if ( COMPRESS_FLAG ) {
            char* compressed_file = compress_file( argv[j] );
            append_to_archive( compressed_file );
          } else {
            append_to_archive( argv[j] );
          }
        }
      }
      if ( flag == 'c' ) {
        init_archive();
        if ( COMPRESS_FLAG ) { println( "Compressing files" ); }
        if ( !COMPRESS_FLAG ) { println( "Not compressing files" ); }
        assert( COMPRESS_FLAG );
        int j;
        for ( j=i+1; j < argc; j++ ) { // iterate through list of files/directories present in argv
          if ( COMPRESS_FLAG ) {
            char* compressed_file = compress_file( argv[j] );
            append_to_archive( compressed_file );
          } else {
            append_to_archive( argv[j] );
          }
        }
      }
      if ( flag == 'x' ) {
        init_archive();
        decompress_all();
      } // unarchive (extract -> decompress)
      if ( flag == 'r' ) {
        init_archive();
        read_files();
      }
      if ( flag == 'k' ) {
        println( "Decompressing files (command line args)" );
        int j;
        for ( j=i+1; j < argc; j++ ) // iterate through list of files/directories present in argv
          decompress_file( argv[j] );
      }
    }
  }

  fclose( disk );
  return 0;
 
}