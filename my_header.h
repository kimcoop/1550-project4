
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
#include <fcntl.h>
#include <assert.h>

#define MEDIUM_BUFFER 32
#define SMALL_BUFFER 12
#define TRUE 1
#define FALSE 0
#define OUTPUT_FILE "output.txt"
#define TEST_FILE "testfile.txt"

#define BLOCK_SIZE 512 //size of a disk block
#define MAX_FILENAME 11
#define MODE_SIZE 3
#define MAX_FILES_IN_DIR (BLOCK_SIZE - (MAX_FILENAME + 1) - sizeof(int)) / ((MAX_FILENAME + 1) + sizeof(size_t) + sizeof(long))

#define MAX_DATA_IN_BLOCK (BLOCK_SIZE - sizeof(unsigned long))



/* DEBUGGING -  SET THIS VALUE TO 1 TO LOG OUTPUT */
#define DEBUG 0

/* DEBUGGING -  SET THIS VALUE TO 1 TO LOG OUTPUT */
#define println(...) if ( DEBUG ) { printf("%d:\t", __LINE__); }  printf( __VA_ARGS__ ); printf("\n")
#define log(...) if ( DEBUG ) { printf("%d:\t", __LINE__); printf( __VA_ARGS__ ); printf("\n"); }
#define strEqual(a, b) !strcmp(a, b)

/* DEFAULTS (overwritten by command line args) */

/* GLOBALS */

// my_print
int get_file_size( char *name );


//dir
int create_descriptor(char *filename);

// utils.c
int is_dir( struct stat* buf );
void p_stat( char*, struct stat* );
DIR* p_opendir( char* name );

#include "my_print.c"
#include "utils.c"
#include "dir.c"
// #include "disk.c"