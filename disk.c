#ifndef DISK_H
#define DISK_H

//opens some random disk
int openDisk(char *filename, int num_bytes);
//creates descriptor
int createDescriptor(char *filename);
//reads blocks
int readBlock(int disk, int block_num, int *block);
//writes blocks
int writeBlock(int disk, int block_num, int *block);
//syncs the disk
void syncDisk();

void open_disk_test();
void write_test();
void read_test();

#endif





#include "my_header.h"

#define DISKFULL -1

int disk = 0;
int disk_descriptor;
int size_bytes;

int openDisk(char *filename, int num_bytes) {
  size_bytes = num_bytes;
  disk_descriptor = createDescriptor("Disk");

  println( "openDisk ");

  return 1;
}

//creates a descriptor that openDisk can call
int createDescriptor(char *filename) {
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
      new_descriptor = open(filename, flags);
      return new_descriptor;
      printf("lol\n");
      break;
    case EACCES:
      printf("Permission denied.\n");
      break;
    case EMFILE:
      printf("No file handle available. \n");
      break;
  }

  println(" createDescriptor ");
  return descriptor;
}


int readBlock(int disk, int block_num, int *block) {
  int blocksize = 1024;
  int position = block_num * blocksize;
  println(" readBlock ");

  //move file pointer
  // lseek(disk_descriptor, position, SEEK_SET);

  pread(disk_descriptor, block, blocksize, position);

  return 1;
}


int writeBlock(int disk, int block_num, int *block) {

  int blocksize = 1024;
  int *contents = malloc(1024);

  println("writeBlock block_num %d", block_num );
  if (block_num * blocksize > size_bytes) {
    printf("The disk is full.\n");
    return DISKFULL;
  }

  //to find the position in the individual disk where the block should be written
  // int position = (block_num / 3) * blocksize;
  int position = block_num * blocksize;

  pwrite(disk_descriptor, block, blocksize, position);
  // lseek(disk_descriptor, position, SEEK_SET);
  // pread(descriptor2, contents, blocksize, position);
  return 1;

}


void syncDisk() {
  println(" syncDisk ");
  // _flushlbf();
}


int main() {
  open_disk_test();
  write_test();
  read_test();
  return 0;
}

//this function tests opening a disk and printing how big those disks are allowed to be
void open_disk_test() {

  int test_bytes = 300;
  FILE *filetest = fopen("Disk", "r");
  if (filetest == NULL) {
    perror ("The following error occurred");
    printf( "Value of errno: %d\n", errno );
  }
  openDisk("yerrr", test_bytes);
  assert(test_bytes == size_bytes);
  assert(filetest != NULL);
  
}

//tests if the write function works
void write_test() {
  int *test_pointer = malloc(1024);
  int i = 0;

  for(i = 0; i < 256; i++) {
    test_pointer[i] = i * 200;
  }
  writeBlock(1, 1, test_pointer);
}

//writes information to a file and then reads it and compares if they are the same
void read_test() {
  int *test_pointerR = malloc(1024);
  int *test_pointerW = malloc(1024);

  int i = 0;
  for(i = 0; i < 256; i++) {
    test_pointerW[i] = i * 200;
  }
  writeBlock(1, 11, test_pointerW);
  readBlock(1, 11, test_pointerR);

  for (i = 0; i < 256; i++) {
    // printf("position %d\n", i);
    // printf("%d, %d\n", test_pointerW[i], test_pointerR[i]);
    assert(test_pointerW[i] == test_pointerR[i]);
  }
}
