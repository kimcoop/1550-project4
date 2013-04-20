
OBJS 	= pittar compress
CC	= gcc
OTHER_FLAGS = -o

all: clean pittar compress

pittar: pittar.c
	$(CC) pittar.c $(OTHER_FLAGS) pittar

compress: compress.c
	$(CC) compress.c $(OTHER_FLAGS) compress

# clean house
clean:
	rm -f $(OBJS) $(OUT)
