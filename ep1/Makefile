COMPILER := g++
COM_FLAGS := -g -c
LINKER := g++
LNK_FLAGS := -lglfw

all: hellovk

hellovk: main.o
	${LINKER} ${LNK_FLAGS} -o hellovk main.o

main.o: main.cc
	${COMPILER} ${COM_FLAGS} main.cc -o main.o

clean:
	rm -rf *.o
	rm -rf hellovk
