

#define FILE_NOT_FOUND 0
#define FILE_FOUND 1

void decompress_file( char* filename ) {
  println(" decompressing %s", filename );

  if ( !file_exists(filename) ) {
    println( "Error: cannot compress file %s--File not found.", filename );
    exit(0);
  }

  pid_t childpid;
  int status;

  char *const argv[] = {
    "compress",
    "-d", 
    filename,
    (char*) 0
  };
    
  if ( (childpid = fork()) < 0 ) 
    perror("Failed to fork");

  if ( childpid == 0 ) { 
    if ( execv( "./compress", argv ) == -1 ) 
      perror( "execv " );
  }
  
  wait( &status );

}


// compresses file and returns compressed file name (.Z added), or if it doesn't exist, original file name
char* compress_file( char* filename ) {

  if ( !file_exists(filename) ) {
    println( "Error: cannot compress file %s--File not found.", filename );
    exit(0);
  }

  println("compressing %s", filename);
  
  pid_t childpid;
  int status;

  char *const argv[] = {
    "compress",
    filename,
    (char*) 0
  };
    
  if ( (childpid = fork()) < 0 ) 
    perror("Failed to fork");

  if ( childpid == 0 ) { 
    if ( execv( "./compress", argv ) == -1 ) 
      perror( "execv " );
  }
  
  wait( &status );
  
  char *ext = ".Z";
  char* new_name = (char *) malloc( strlen(filename) + strlen(ext)+1 );

  new_name[0] = '\0';
  strcpy( new_name, filename );
  strcat( new_name, ext );
  println( "%s", new_name );
  
  if ( file_exists( new_name ) ) {
    println("file %s exists - compressed", new_name);
    return new_name;
  } else {
    println("file %s compression didn't work", new_name );
    return filename;
  }

}

// checks if a file exists 
int file_exists( const char *name) {

  FILE *f;
  if ( f = fopen(name, "r") ) {
    fclose(f);
    return FILE_FOUND;
  }
  return FILE_NOT_FOUND;

}
