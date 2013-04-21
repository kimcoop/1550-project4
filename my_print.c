
void list_meta_data( char *name ) {

  struct dirent *dir;
  char  *newname;
  struct stat mybuf;
  DIR *dp = p_opendir( name );

  while ( (dir = readdir(dp)) != NULL ) {

    if ( dir->d_ino == 0 )
      continue;

    if ( dir->d_name[0] != '.' ) {
    
      newname=(char *)malloc(strlen(name)+strlen(dir->d_name)+2);

      strcpy(newname,name);
      strcat(newname,"/");
      strcat(newname, dir->d_name);


      p_stat( newname, &mybuf );


      if ( is_dir(&mybuf) ) {
        meta_data( newname );
        list_meta_data( newname );
      } else {
        meta_data( newname );
      }

      free( newname );
      newname = NULL;

    }
  } // while

  closedir( dp );
}

int get_file_size( char *name ) {

  struct stat mybuf;
  p_stat( name, &mybuf );

  return (int) mybuf.st_size;

}

int get_perms( char *name ) {

  struct stat mybuf;
  p_stat( name, &mybuf );

  return mybuf.st_mode;
}

void print_perms( int mode ) {
  
  char type, perms[10];
  int i,j;

  *perms='\0';

  for ( i=2; i>=0; i-- ) {
    j = ( mode >> (i*3)) & 07;
    strcat( perms,modes[j] ); 
  }

  printf( "%s", perms );

}

void meta_data( char *name ) {

  struct stat mybuf;
  char    type, perms[10];
  int     i,j;

  p_stat( name, &mybuf );
  switch (mybuf.st_mode & S_IFMT) {
    case S_IFREG: type = '-'; break;
    case S_IFDIR: type = 'd'; break;
    default:      type = '?'; break;
  }

  *perms='\0';

  for(i=2; i>=0; i--){
    j = (mybuf.st_mode >> (i*3)) & 07;
    strcat(perms,modes[j]); 
  }

  printf("%c%s%3d %5d/%-5d %7d %.12s %s \n",\
    type, perms, mybuf.st_nlink, mybuf.st_uid, mybuf.st_gid, \
    (int)mybuf.st_size, ctime(&mybuf.st_mtime)+4, name);
} 


void tabify( int n ) {
  int i;
  for ( i = 0; i <= n; i++ ) {
    printf("\t");
  }
}


void hierarchy(char *name, int level ) {

  struct dirent *dir;
  char 	*newname;
  struct stat mybuf;
  DIR *dp = p_opendir( name );

	while ( (dir = readdir(dp)) != NULL ) {

		if ( dir->d_ino == 0 )
      continue;

    if ( dir->d_name[0] != '.' ) {
    
      newname=(char *)malloc(strlen(name)+strlen(dir->d_name)+2);

      strcpy(newname,name);
      strcat(newname,"/");
  		strcat(newname, dir->d_name);
      p_stat( newname, &mybuf );

      if ( level )
        tabify( level );

      if ( is_dir(&mybuf) ) {
        printf("%s/\n", dir->d_name );
        hierarchy( newname, level+1 );
      } else {
        printf("%s\n", dir->d_name );
      }

  		free( newname );
      newname = NULL;

    }
	} // while

	closedir( dp );
}



void print_archive() {

  println( "Directory name: %s", archive.header.d_name );
  println( "Number of files: %d\n", archive.header.num_files );

  int i;
  for ( i=0; i < archive.header.num_files; i++ ) {
    println("---FILE %d---", i );
    println( "name: %s", archive.meta_data[ i ].f_name );
    println( "size: %lu", archive.meta_data[ i ].f_size );
    println( "start: %lu", archive.meta_data[ i ].f_start );
  }
}

void print_archive_meta_data() {
  println( "Directory name: %s", archive.header.d_name );
  println( "Number of files: %d\n", archive.header.num_files );

  int i;
  for ( i=0; i < archive.header.num_files; i++ ) {
    println("---FILE %d---", i );
    println( "name: %s", archive.meta_data[ i ].f_name );
    println( "size: %lu", archive.meta_data[ i ].f_size );
    printf( "modes: " );
    print_perms( archive.meta_data[ i ].f_modes );
    printf( "\n" );

  }
}

