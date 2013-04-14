
void list(char *, int);
void tabify( int );
void printout(char *);

char *modes[]={"---","--x","-w-","-wx","r--","r-x","rw-","rwx"}; // eight distinct modes


void printout(char *name) {

  struct stat   mybuf;
  char    type, perms[10];
  int     i,j;

  stat(name, &mybuf);
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


void list(char *name, int level ){
  
  DIR 	*dp;
  struct dirent *dir;
  char 	*newname;
  struct stat mybuf;
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
    // if ( !strEqual(dir->d_name, ".") && !strEqual(dir->d_name, "..") && !strEqual(dir->d_name, ".git") ) {
    if ( dir->d_name[0] != '.' ) {
    
      newname=(char *)malloc(strlen(name)+strlen(dir->d_name)+2);

      strcpy(newname,name);
      strcat(newname,"/");
  		strcat(newname, dir->d_name);
      p_stat( newname, &mybuf );

      if ( level ) {
        tabify( level ); 
      }

      if ( is_dir(&mybuf) )
        printf("%s/\n", dir->d_name );
      else
        printf("%s\n", dir->d_name );

      if ( is_dir(&mybuf) )
        list( newname, level+1 );

  		free(newname);
      newname=NULL;

    }
	}

	closedir( dp );
}


