cmake_minimum_required(VERSION 3.16.0)
project(nbcraft-renderer-d3d)

# Build
add_library(nbcraft-renderer-d3d STATIC
    API_D3D.cpp
    RenderContextD3D.cpp
    RenderDeviceD3D.cpp
    ShaderProgramD3D.cpp
)

# Libraries
target_link_libraries(nbcraft-renderer-d3d
    nbcraft-core
)

# Headers
target_include_directories(nbcraft-renderer-d3d PUBLIC .)
