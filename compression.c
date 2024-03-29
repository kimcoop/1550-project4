

#define FILE_NOT_FOUND 0
#define FILE_FOUND 1

void decompress_file( char* filename ) {

  if ( !file_exists(filename) ) {
    println( "Error: cannot compress file %s (file not found)", filename );
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

char* compress_file( char* filename ) {

  if ( !file_exists(filename) ) {
    println( "Error: cannot compress file %s (file not found)", filename );
    exit(0);
  }

  log("Compressing file %s", filename);
  
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
    log("File %s compressed successfully.", new_name);
    return new_name;
  } else {
    log("Error: file %s compression failed.", new_name );
    return filename;
  }
  return filename;

}

int file_exists( const char *name) {

  FILE *f;
  if ( f = fopen(name, "r") ) {
    fclose(f);
    return FILE_FOUND;
  }
  return FILE_NOT_FOUND;

}
