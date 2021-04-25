COMPILER := g++
COM_FLAGS := -g -c
LINKER := g++
LNK_FLAGS := -lglfw -lvulkan

LNK_DEPS := main.o Application.o AppSwapchain.o

all: hellovk

hellovk: ${LNK_DEPS}
	${LINKER} ${LNK_FLAGS} -o hellovk ${LNK_DEPS}

main.o: main.cc ${DEPS}
	${COMPILER} ${COM_FLAGS} main.cc -o main.o

Application.o: Application.cc Application.hh
	${COMPILER} ${COM_FLAGS} Application.cc -o Application.o

AppSwapchain.o: AppSwapchain.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppSwapchain.cc -o AppSwapchain.o

clean:
	rm -rf *.o
	rm -rf hellovk
