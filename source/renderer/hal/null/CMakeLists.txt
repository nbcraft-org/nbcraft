cmake_minimum_required(VERSION 3.16.0)
project(nbcraft-renderer-null)

# Build
add_library(nbcraft-renderer-null STATIC
    BlendStateNull.cpp
    BufferNull.cpp
    ConstantBufferContainerNull.cpp
    DepthStencilStateNull.cpp
    ImmediateBufferNull.cpp
    RasterizerStateNull.cpp
    RenderContextNull.cpp
    RenderDeviceNull.cpp
    ShaderConstantNull.cpp
    ShaderConstantWithDataNull.cpp
    ShaderNull.cpp
    ShaderProgramNull.cpp
    TextureNull.cpp
)

# Libraries
target_link_libraries(nbcraft-renderer-null
    nbcraft-core
)

# Headers
target_include_directories(nbcraft-renderer-null PUBLIC .)
