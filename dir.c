
#define BLOCK_SIZE 512 //size of a disk block
#define MAX_FILENAME 8
#define MAX_EXTENSION 3
#define MODE_SIZE 3
#define MAX_FILES_IN_DIR (BLOCK_SIZE - (MAX_FILENAME + 1) - sizeof(int)) / \
  ((MAX_FILENAME + 1) + (MAX_EXTENSION + 1) + sizeof(size_t) + sizeof(long))

#define MAX_DATA_IN_BLOCK (BLOCK_SIZE - sizeof(unsigned long))


typedef struct {

  char f_name[ MAX_FILENAME +1 ];
  char f_ext[ MAX_EXTENSION +1 ];
  size_t f_size;
  char modes[ MODE_SIZE +1 ];
  long start; // where first data is found on disk

} MetaData;

typedef struct {

  char d_name[ MEDIUM_BUFFER +1 ];
  int num_files;
  char data_block[ MAX_DATA_IN_BLOCK ];
  MetaData* meta_data[ MAX_FILES_IN_DIR ]; // array of pointers to meta_data

} Archive;