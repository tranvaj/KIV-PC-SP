CC=gcc
CFLAGS=-Wall -pedantic -g
ENDFLAGS=-lm
OBJS=hashtable.o main.o bayes.o filereader.o
EXE=spamid.exe

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(ENDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) -c $^ $(ENDFLAGS)
	