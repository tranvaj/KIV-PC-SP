CC=gcc
CFLAGS=-Wall -g
OBJS=hashtable.o main.o
EXE=bayer.EXE

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.c.o:
	$(CC) -c $(CFLAGS) -c $^
	