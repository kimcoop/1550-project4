

int is_dir( struct stat* buf ) {
  return ( buf->st_mode & S_IFMT ) == S_IFDIR;
}

void p_stat( char* f, struct stat* buf ) {
  if (stat( f, buf ) < 0) { 
    perror( f );
  }
}

DIR* p_opendir( char* name ) {

  DIR *dp;
  if ( (dp = opendir( name )) == NULL ) {
    perror("opendir"); 
    return;
  }
  return dp;

}