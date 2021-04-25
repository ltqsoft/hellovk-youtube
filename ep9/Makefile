COMPILER := g++
COM_FLAGS := -g -c
LINKER := g++
LNK_FLAGS := -lglfw -lvulkan

LNK_DEPS := obj/Application.o obj/AppSwapchain.o obj/AppRenderPass.o

all: hellovk

hellovk: obj/main.o ${LNK_DEPS}
	${LINKER} ${LNK_FLAGS} -o hellovk obj/main.o ${LNK_DEPS}

obj/main.o: main.cc ${LNK_DEPS}
	${COMPILER} ${COM_FLAGS} main.cc -o obj/main.o

obj/Application.o: Application.cc Application.hh
	${COMPILER} ${COM_FLAGS} Application.cc -o obj/Application.o

obj/AppSwapchain.o: AppSwapchain.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppSwapchain.cc -o obj/AppSwapchain.o

obj/AppRenderPass.o: AppRenderPass.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppRenderPass.cc -o obj/AppRenderPass.o

clean:
	rm -rf *.o
	rm -rf obj/*.o
	rm -rf hellovk
