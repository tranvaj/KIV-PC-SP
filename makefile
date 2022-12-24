CC=gcc
CFLAGS=-Wall -g
ENDFLAGS=-lm
OBJS=hashtable.o main.o bayes.o
EXE=bayer.EXE

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(ENDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) -c $^ $(ENDFLAGS)
	