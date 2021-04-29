COMPILER := g++
COM_FLAGS := -Wall -g -c
COM_DEPS := Application.hh data.hh
LINKER := g++
LNK_FLAGS := -lglfw -lvulkan

LNK_DEPS := obj/Application.o obj/AppSwapchain.o obj/AppRenderPass.o \
obj/AppCmds.o obj/AppShaders.o obj/AppBuffers.o obj/AppDescriptors.o \
obj/AppUpdate.o obj/AppViewer.o obj/AppKeyboard.o obj/AppInit.o \
obj/AppImages.o

all: cxx spirv

cxx: hellovk

hellovk: obj/main.o ${LNK_DEPS}
	${LINKER} ${LNK_FLAGS} -o hellovk obj/main.o ${LNK_DEPS}

obj/main.o: main.cc ${LNK_DEPS}
	${COMPILER} ${COM_FLAGS} main.cc -o obj/main.o

obj/Application.o: Application.cc ${COM_DEPS}
	${COMPILER} ${COM_FLAGS} Application.cc -o obj/Application.o

obj/AppBuffers.o: AppBuffers.cc ${COM_DEPS}
	${COMPILER} ${COM_FLAGS} AppBuffers.cc -o obj/AppBuffers.o

obj/AppCmds.o: AppCmds.cc ${COM_DEPS}
	${COMPILER} ${COM_FLAGS} AppCmds.cc -o obj/AppCmds.o

obj/AppDescriptors.o: AppDescriptors.cc ${COM_DEPS}
	${COMPILER} ${COM_FLAGS} AppDescriptors.cc -o obj/AppDescriptors.o

obj/AppImages.o: ${COM_DEPS} AppImages.cc
	${COMPILER} ${COM_FLAGS} AppImages.cc -o obj/AppImages.o

obj/AppInit.o: ${COM_DEPS} AppInit.cc
	${COMPILER} ${COM_FLAGS} AppInit.cc -o obj/AppInit.o

obj/AppKeyboard.o: ${COM_DEPS} AppKeyboard.cc
	${COMPILER} ${COM_FLAGS} AppKeyboard.cc -o obj/AppKeyboard.o

obj/AppSwapchain.o: AppSwapchain.cc ${COM_DEPS}
	${COMPILER} ${COM_FLAGS} AppSwapchain.cc -o obj/AppSwapchain.o

obj/AppRenderPass.o: AppRenderPass.cc ${COM_DEPS}
	${COMPILER} ${COM_FLAGS} AppRenderPass.cc -o obj/AppRenderPass.o

obj/AppShaders.o: AppShaders.cc ${COM_DEPS}
	${COMPILER} ${COM_FLAGS} AppShaders.cc -o obj/AppShaders.o

obj/AppUpdate.o: AppUpdate.cc ${COM_DEPS}
	${COMPILER} ${COM_FLAGS} AppUpdate.cc -o obj/AppUpdate.o

obj/AppViewer.o: ${COM_DEPS} AppViewer.cc
	${COMPILER} ${COM_FLAGS} AppViewer.cc -o obj/AppViewer.o

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
