cmake_minimum_required(VERSION 3.16.0)
project(nbcraft-renderer-dxgi)

# Build
add_library(nbcraft-renderer-dxgi STATIC
    API_DXGI.cpp
    helpers/ErrorHandlerDXGI.cpp
	helpers/TextureHelperDXGI.cpp
)

# Libraries
target_link_libraries(nbcraft-renderer-dxgi
    nbcraft-core
    dxgi
)

# Headers
target_include_directories(nbcraft-renderer-dxgi PUBLIC .)
