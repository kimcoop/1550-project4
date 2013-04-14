
char *modes[]={"---","--x","-w-","-wx","r--","r-x","rw-","rwx"}; // eight distinct modes

void print_hierarchy( char*);
void list(char *);
void printout(char *);

void print_hierarchy( char *archive_file ) {

  struct stat mybuf;

  if ( archive_file == NULL || strEqual(archive_file, "") ) { 
    list("."); 
    exit(0);
  }

  while( archive_file != NULL ) {
    p_stat( archive_file, &mybuf );

    if ((mybuf.st_mode & S_IFMT) == S_IFDIR )
      list( archive_file );      // directory encountered
      // print_hierarchy( archive_file );      // directory encountered
    else 	
      printout( archive_file );  // file encountered
  } 

} // main


void list(char *name){
  DIR 	*dp;
  struct dirent *dir;
  char 	*newname;
  dp = opendir(name);

	if ( dp== NULL ) {
		perror("opendir"); 
    return;
	}

	while ((dir = readdir(dp)) != NULL ) {
		if (dir->d_ino == 0 ) {
      continue;
      println("dir name: %s", dir->d_name );
    }

    if ( !strEqual(dir->d_name, ".") && !strEqual(dir->d_name, "..") ) {
    
  		newname=(char *)malloc( strlen(dir->d_name)+2 );
  		strcpy(newname,"\t");
  		strcat(newname, dir->d_name);
      println( "%s", newname );
  		free(newname);
      newname=NULL;
    }
	}

	closedir( dp);
} // void list(char *name){


void printout(char *name){
  struct stat 	mybuf;
  char 		type, perms[10];
  int 		i,j;

	stat(name, &mybuf);
	switch (mybuf.st_mode & S_IFMT){
  	case S_IFREG: 
      type = '-'; 
      break;
  	case S_IFDIR: 
      type = 'd'; 
      // print_hierarchy( name );
      break;
  	default:      
      type = '?'; 
      break;
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



