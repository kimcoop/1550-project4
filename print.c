
char *modes[]={"---","--x","-w-","-wx","r--","r-x","rw-","rwx"}; // eight distinct modes

void list(char *);
void printout(char *);

// main(int argc, char *argv[]){
void print_hierarchy( char *archive_file ) {

  struct stat mybuf;
  int argc = 0; // TODO
  char** argv = NULL;

  if (argc<2) { 
    list("."); 
    exit(0);
  }

  while(--argc){
    if (stat(*++argv, &mybuf) < 0) { 
  	 perror(*argv); continue;
  	}

    if ((mybuf.st_mode & S_IFMT) == S_IFDIR )
      list(*argv);      // directory encountered
    else 	
      printout(*argv);  // file encountered
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
		if (dir->d_ino == 0 ) 
      continue;
		newname=(char *)malloc(strlen(name)+strlen(dir->d_name)+2);
		strcpy(newname,name);
		strcat(newname,"/");
		strcat(newname,dir->d_name);
		printout(newname);
		free(newname); newname=NULL;
	}
	close( dp);
} // void list(char *name){

void printout(char *name){
  struct stat 	mybuf;
  char 		type, perms[10];
  int 		i,j;

	stat(name, &mybuf);
	switch (mybuf.st_mode & S_IFMT){
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
} //void printout(char *name){



