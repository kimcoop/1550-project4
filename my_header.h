
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEDIUM_BUFFER 32
#define SMALL_BUFFER 12
#define TRUE 1
#define FALSE 0
#define OUTPUT_FILE "output.txt"

/* DEBUGGING -  SET THIS VALUE TO 1 TO LOG OUTPUT */
#define DEBUG 0

/* DEBUGGING -  SET THIS VALUE TO 1 TO LOG OUTPUT */
#define println(...) if ( DEBUG ) { printf("%d:\t", __LINE__); }  printf( __VA_ARGS__ ); printf("\n")
#define log(...) if ( DEBUG ) { printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n"); }
#define strEqual(a, b) !strcmp(a, b)

/* DEFAULTS (overwritten by command line args) */

/* GLOBALS */

// utils.c
int is_dir( struct stat* buf );
void p_stat( char*, struct stat* );
DIR* p_opendir( char* name );

#include "my_print.c"
#include "utils.c"