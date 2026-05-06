cmake_minimum_required(VERSION 3.16.0)
project(nbcraft-renderer-d3d11)

# Build
add_library(nbcraft-renderer-d3d11 STATIC
    API_D3D11.cpp
    BlendStateD3D11.cpp
    BufferD3D11.cpp
    ConstantBufferContainerD3D11.cpp
    DepthStencilStateD3D11.cpp
    GPUEventsD3D11.cpp
    ImmediateBufferD3D11.cpp
    ProfileSectionD3D11.cpp
    RasterizerStateD3D11.cpp
    RenderContextD3D11.cpp
    RenderDeviceD3D11.cpp
    ShaderConstantD3D11.cpp
    ShaderConstantWithDataD3D11.cpp
    ShaderD3D11.cpp
    ShaderProgramD3D11.cpp
    ShaderResourceD3D11.cpp
    TextureD3D11.cpp
    helpers/TextureHelperD3D11.cpp
)

# Reference Renderer.D3D
add_subdirectory(../d3d nbcraft-renderer-d3d)
target_link_libraries(nbcraft-renderer-d3d11 nbcraft-renderer-d3d)

# Reference Renderer.DXGI
add_subdirectory(../dxgi nbcraft-renderer-dxgi)
target_link_libraries(nbcraft-renderer-d3d11 nbcraft-renderer-dxgi)

# Libraries
target_link_libraries(nbcraft-renderer-d3d11
    nbcraft-core
#    d3d11
#    d3dcompiler
#    dxguid
)

# Headers
target_include_directories(nbcraft-renderer-d3d11 PUBLIC .)
