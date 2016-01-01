CC=clang++
CFLAGS=-std=c++11 -w -g 

all: main.cpp alloc.o testAlloc.o
	$(CC) $(CFLAGS) -o alloc.out $^

alloc.o : alloc.cpp alloc.h block.h
	$(CC) $(CFLAGS) -c -o $@ $<

testAlloc.o: TestAlloc.cpp TestAlloc.h alloc.h block.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o
	rm *.out
