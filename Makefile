
OBJS 	= pittar
CC	= gcc
OTHER_FLAGS = -o

all: pittar

pittar: pittar.c
	$(CC) pittar.c $(OTHER_FLAGS) pittar

# clean house
clean:
	rm -f $(OBJS) $(OUT)
