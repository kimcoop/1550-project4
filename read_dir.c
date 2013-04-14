

int is_dir( struct stat* buf ) {
  return ( buf->st_mode & S_IFMT ) == S_IFDIR;
}

void p_stat( char* f, struct stat* buf ) {
  if (stat( f, buf ) < 0) { 
    perror( f );
  }
}

void do_ls( char dirname[] ){
  DIR   *dir_ptr;
  struct  dirent *direntp;
  struct stat mybuf;

  if ( ( dir_ptr = opendir( dirname ) ) == NULL ) {
    println( "cannot open %s \n",dirname);
  } else {

    while ( ( direntp=readdir(dir_ptr) ) != NULL ) {
      p_stat( direntp->d_name, &mybuf );
      if ( is_dir(&mybuf) ) {
        println("%s/", direntp->d_name);
        list( direntp->d_name );
      }
      
    } // while
    closedir(dir_ptr);

  }
}

int read_dir( char* dir) {
  println(" read_dir ");
  int i = 0;
  if ( !dir || strEqual( dir, "" )) 
    do_ls(".");
  else while ( dir && i < 10){
      printf("%s: \n",dir ) ;
      do_ls( dir );
    }
} 


// int read_dir( int argc, char *argv[] ) {
//   if (argc == 1 ) 
//     do_ls(".");
//   else while ( --argc ){
//       printf("%s: \n", *++argv ) ;
//       do_ls(*argv);
//     }
// } 

