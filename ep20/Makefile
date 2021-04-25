COMPILER := g++
COM_FLAGS := -Wall -g -MMD -c
LINKER := g++
LNK_FLAGS := -lglfw -lvulkan

LNK_DEPS := obj/Application.o obj/AppSwapchain.o obj/AppRenderPass.o \
obj/AppCmds.o obj/AppShaders.o obj/AppBuffers.o obj/AppDescriptors.o \
obj/AppUpdate.o

all: hellovk

hellovk: obj/main.o ${LNK_DEPS}
	${LINKER} ${LNK_FLAGS} -o hellovk obj/main.o ${LNK_DEPS}

obj/main.o: main.cc ${LNK_DEPS}
	${COMPILER} ${COM_FLAGS} main.cc -o obj/main.o

obj/Application.o: Application.cc Application.hh
	${COMPILER} ${COM_FLAGS} Application.cc -o obj/Application.o

obj/AppDescriptors.o: AppDescriptors.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppDescriptors.cc -o obj/AppDescriptors.o

obj/AppSwapchain.o: AppSwapchain.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppSwapchain.cc -o obj/AppSwapchain.o

obj/AppRenderPass.o: AppRenderPass.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppRenderPass.cc -o obj/AppRenderPass.o

obj/AppBuffers.o: AppBuffers.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppBuffers.cc -o obj/AppBuffers.o

obj/AppCmds.o: AppCmds.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppCmds.cc -o obj/AppCmds.o

obj/AppShaders.o: AppShaders.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppShaders.cc -o obj/AppShaders.o

obj/AppUpdate.o: AppUpdate.cc Application.hh
	${COMPILER} ${COM_FLAGS} AppUpdate.cc -o obj/AppUpdate.o

spirv: basic-vert.spirv basic-frag.spirv

basic-vert.spirv: shaders/basic.vert
	glslangValidator -S vert -o basic-vert.spirv -V shaders/basic.vert

basic-frag.spirv: shaders/basic.frag
	glslangValidator -S frag -o basic-frag.spirv -V shaders/basic.frag

clean:
	rm -rf obj/*.o
	rm -rf hellovk

clean/spirv:
	rm -rf *.spirv
