# CC=/opt/homebrew/Cellar/llvm/17.0.6_1/bin/clang

CFLAGS = -g -Wall -std=c99 -fopenmp
CXXFLAGS = -g -Wall -fopenmp
LDFLAGS = -fopenmp
DEPS=tree.h

all: main.o tree.o
	$(CC) $(LDFLAGS) -o test main.o tree.o

*.o: *.c $(DEPS)
	$(CC) $(CFLAGS) -c %<.c -o $@.o

clean:
	rm -rf *.o test
