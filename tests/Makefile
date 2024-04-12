CC = clang++

CFLAGS = -S -emit-llvm

all: $(patsubst %.cpp,%.ll,$(wildcard *.cpp))

%.ll: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.ll
