int open_disk(char *filename, int num_bytes);
int create_descriptor(char *filename);
int read_block( int *block);
int write_block( int *block);

void open_disk_test();
void write_test();
void read_test();



// typedef struct directoryNode{
//   char* name;
//   int numberOfChildren;
//   // int inodeNumber;
//   struct directoryNode* parent;
//   struct directoryNode* children[MAX_CHILDREN];
// }directoryNode_t;

// directoryNode_t* root;


#include "my_header.h"

#define DISKFULL -1
// #define BLOCK_SIZE 1024

int disk = 0;
int disk_descriptor;
int size_bytes;

int open_disk(char *filename, int num_bytes) {
  size_bytes = num_bytes;
  disk_descriptor = create_descriptor("Disk");

  println( "open_disk ");

  return 1;
}

//creates a descriptor that open_disk can call
int create_descriptor(char *filename) {
  int flags;
  flags = O_RDWR;
  int descriptor = open(filename, flags);

  FILE *fp;
  int new_descriptor;

  switch(errno) {
    //file doesn't exist so create binary file and then open it with open
    case ENOENT:
      println(" needing to create a new file ");
      fp = fopen(filename,"wb");
      fclose(fp);
      descriptor = open(filename, flags);
      break;
    case EACCES:
      printf("Permission denied.\n");
      break;
    case EMFILE:
      printf("No file handle available. \n");
      break;
  }

  return descriptor;
}


int read_block( int *block ) {

  // int position = BLOCK_SIZE;
  int position = 0;
  println(" read_block ");

  pread( disk_descriptor, block, BLOCK_SIZE, position );

  int i = 0;
  for(i = 0; i < 50; i++) {
    // printf("%c", block[i]);
  }
  println("");

  return 1;
}


int write_block( int *block ) {

  int *contents = malloc( BLOCK_SIZE );
  
  // if ( BLOCK_SIZE > size_bytes ) {
  //   printf("The disk is full.\n");
  //   return DISKFULL;
  // }
  
  // int position = BLOCK_SIZE;
  int position = 0;
  pwrite( disk_descriptor, block, BLOCK_SIZE, position );

  return 1;

}



int main() {
  open_disk_test();
  // write_test();
  read_test();
  return 0;
}

//this function tests opening a disk and printing how big those disks are allowed to be
void open_disk_test() {

  int test_bytes = 300;
  open_disk("yerrr", test_bytes);
  FILE *filetest = fopen("Disk", "r");
  // if (filetest == NULL) {
  //   perror("The following error occurred");
  //   printf( "Value of errno: %d\n", errno );
  // }
  assert(test_bytes == size_bytes);
  assert(filetest != NULL);
  
}

//tests if the write function works
void write_test() {
  int *test_pointer = malloc(BLOCK_SIZE);
  int i = 0;

  for(i = 0; i < 256; i++) {
    test_pointer[i] = i;
  }
  write_block( test_pointer );
}

//writes information to a file and then reads it and compares if they are the same
void read_test() {
  int *test_pointer_read = malloc(BLOCK_SIZE);
  int *test_pointer_write = malloc(BLOCK_SIZE);

  int i = 0;
  for(i = 0; i < 50; i++) {
    test_pointer_write[i] = 'a';
  }
  write_block( test_pointer_write );
  read_block( test_pointer_read );

  for (i = 0; i < 50; i++) {
    // printf("position %d\n", i);
    // printf("%d, %d\n", test_pointer_write[i], test_pointer_read[i]);
    assert(test_pointer_write[i] == test_pointer_read[i]);
  }
}








