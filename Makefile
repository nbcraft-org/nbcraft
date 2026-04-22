# Makefile build
# meant to be extremely portable to weird unix-like systems

CC := cc
CXX := c++
AR := ar

CFLAGS := -O2 -DNDEBUG
CXXFLAGS := -O2 -DNDEBUG

DEFINES := -DHANDLE_CHARS_SEPARATELY -DRAPIDJSON_NO_THREAD_LOCAL -DSTBI_NO_THREAD_LOCALS
INCLUDES := -I. -Isource -Ithirdparty/zlib -Ithirdparty/raknet -Ithirdparty/rapidjson -Ithirdparty/stb_image/include -I/usr/X11R6/include
LIBS := -L/usr/X11R6/lib -pthread

HEADERS := $(wildcard compat/*.h) \
           $(wildcard compat/*.hpp) \
           $(wildcard thirdparty/stb_image/include/*.h) \
           $(wildcard thirdparty/raknet/*.h) \
           $(wildcard thirdparty/zlib/*.h) \
           $(shell find source -name '*.hpp') \
           $(shell find source -name '*.h') \
           $(shell find platforms -name '*.hpp') \
           $(shell find platforms -name '*.h') \
           $(shell find thirdparty/rapidjson -name '*.h')
C_SRCS := $(wildcard thirdparty/zlib/*.c) thirdparty/stb_image/src/stb_image_impl.c thirdparty/stb_image/include/stb_vorbis.c
CXX_SRCS := $(shell find source \
    -path source/renderer/platform -prune -o \
    -path source/renderer/hal/ogl -prune -o \
    -path source/renderer/hal/d3d11 -prune -o \
    -path source/renderer/hal/d3d9 -prune -o \
    -path source/renderer/hal/dxgi -prune -o \
    -path source/renderer/hal/null -prune -o \
    -name '*.cpp' -print) \
    source/renderer/hal/null/AlphaStateNull.cpp \
    source/renderer/hal/null/RenderStateNull.cpp \
    source/renderer/hal/null/FogStateNull.cpp \
    $(wildcard thirdparty/raknet/*.cpp)

# Makefile only supports SDL1 or SDL2 for now, and desktop only
PLATFORM := sdl2
GFX_API := OGL
ifeq ($(PLATFORM),sdl2)
HEADERS += $(wildcard thirdparty/SDL/*.h)
DEFINES += -DUSE_SDL -DUSE_SDL2
LIBS += -lSDL2
else
HEADERS += $(wildcard thirdparty/SDL/*.h)
DEFINES += -DUSE_SDL -DUSE_SDL1
LIBS += -lSDL
endif
CXX_SRCS += platforms/sdl/$(PLATFORM)/main.cpp $(wildcard platforms/sdl/base/*.cpp) $(wildcard platforms/sdl/$(PLATFORM)/base/*.cpp) $(wildcard platforms/sdl/$(PLATFORM)/desktop/*.cpp)
ifeq ($(GFX_API),OGL)
HEADERS += $(wildcard thirdparty/GL/*)
DEFINES += -DMCE_GFX_API_OGL=1
CXX_SRCS += $(shell find source/renderer/hal/ogl -name '*.cpp') $(wildcard source/renderer/platform/ogl/*.cpp)
LIBS += -lGL
else
ifeq ($(GFX_API),OGL_SHADERS)
HEADERS += $(wildcard thirdparty/GL/*)
DEFINES += -DMCE_GFX_API_OGL=1 -DFEATURE_GFX_SHADERS
CXX_SRCS += $(shell find source/renderer/hal/ogl -name '*.cpp') $(wildcard source/renderer/platform/ogl/*.cpp)
LIBS += -lGL
else
ifeq ($(GFX_API),NULL)
DEFINES += -DMCE_GFX_API_NULL=1
# why does the null hal have to have some sources included by default its so dumb
CXX_SRCS += \
    source/renderer/hal/null/BlendStateNull.cpp \
    source/renderer/hal/null/BufferNull.cpp \
    source/renderer/hal/null/ConstantBufferContainerNull.cpp \
    source/renderer/hal/null/DepthStencilStateNull.cpp \
    source/renderer/hal/null/ImmediateBufferNull.cpp \
    source/renderer/hal/null/RasterizerStateNull.cpp \
    source/renderer/hal/null/RenderContextNull.cpp \
    source/renderer/hal/null/RenderDeviceNull.cpp \
    source/renderer/hal/null/ShaderConstantNull.cpp \
    source/renderer/hal/null/ShaderConstantWithDataNull.cpp \
    source/renderer/hal/null/ShaderNull.cpp \
    source/renderer/hal/null/ShaderProgramNull.cpp \
    source/renderer/hal/null/TextureNull.cpp
endif
endif
endif

AUDIO_LIBRARY := openal
INCLUDES += -Iplatforms/audio/$(AUDIO_LIBRARY)
CXX_SRCS += $(wildcard platforms/audio/$(AUDIO_LIBRARY)/*.cpp)
ifeq ($(AUDIO_LIBRARY),openal)
LIBS += -lopenal
endif

ifdef DYNAMIC_OPENGL
DEFINES += -DDYNAMIC_OPENGL=1
endif
ifdef NO_WIDECHAR
DEFINES += -DMC_NO_WSTRING
endif

OBJS := $(addprefix build/,$(C_SRCS:.c=.c.o)) $(addprefix build/,$(CXX_SRCS:.cpp=.cpp.o))

all: build/nbcraft

build/nbcraft: $(OBJS)
	ln -sf ../game/assets build
	$(AR) rcs build/nbcraft.a $(OBJS)
	$(CXX) $(LDFLAGS) build/nbcraft.a $(LIBS) -o build/nbcraft

build/%.cpp.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(DEFINES) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

build/%.c.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(DEFINES) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
