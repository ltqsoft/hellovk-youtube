COMPILER := g++
COM_FLAGS := -g -c
LINKER := g++
LNK_FLAGS := -lglfw -lvulkan

all: hellovk

hellovk: main.o Application.o
	${LINKER} ${LNK_FLAGS} -o hellovk main.o Application.o

main.o: main.cc Application.o
	${COMPILER} ${COM_FLAGS} main.cc -o main.o

Application.o: Application.cc Application.hh
	${COMPILER} ${COM_FLAGS} Application.cc -o Application.o

clean:
	rm -rf *.o
	rm -rf hellovk
