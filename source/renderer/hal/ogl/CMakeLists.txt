cmake_minimum_required(VERSION 3.16.0)
project(nbcraft-renderer-ogl)

# Build
add_library(nbcraft-renderer-ogl STATIC
     API_OGL.cpp
     AlphaStateOGL.cpp
     BlendStateOGL.cpp
     BufferOGL.cpp
     ConstantBufferContainerOGL.cpp
     DepthStencilStateOGL.cpp
     RenderStateOGL.cpp
     FogStateOGL.cpp
     GPUEventsOGL.cpp
     ImmediateBufferOGL.cpp
     ProfileSectionOGL.cpp
     RasterizerStateOGL.cpp
     RenderContextOGL.cpp
     RenderDeviceOGL.cpp
     ResourceOGL.cpp
     ShaderConstantOGL.cpp
     ShaderConstantWithDataOGL.cpp
     ShaderOGL.cpp
     ShaderProgramOGL.cpp
     ShaderResourceOGL.cpp
     ShaderUniformOGL.cpp
     TextureOGL.cpp
     helpers/ErrorHandlerOGL.cpp
     ../../platform/ogl/Extensions.cpp
     ../../platform/ogl/ShaderPrecision.cpp
)

# Libraries
target_link_libraries(nbcraft-renderer-ogl
    nbcraft-core
)

# Headers
target_include_directories(nbcraft-renderer-ogl PUBLIC .)
