
typedef struct {

  char d_name[ MAX_FILENAME +1 ];
  int num_files, space_available;
  long meta_data_pos; // position that dictionary begins from

} Header;

typedef struct {

  char f_name[ MAX_FILENAME +1 ];
  size_t f_size; // file size
  char f_modes[ MODE_SIZE +1 ];
  long f_start; // where first data is found on disk
  // TODO: owner

} MetaData;

typedef struct {

  Header header;
  char data_block[ MAX_DATA_IN_BLOCK ];
  MetaData meta_data[ MAX_FILES_IN_DIR ]; // array of pointers to meta_data

} Archive;
