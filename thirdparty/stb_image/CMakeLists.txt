cmake_minimum_required(VERSION 3.16.0)
project(stb_image)

set(STB_IMAGE_SOURCE src/stb_image_impl.c)

if(NOT NBC_NIX_BUILD)
    set(STB_VORBIS_SOURCE include/stb_vorbis.c)
    set(STB_INCLUDE include)
endif()

# Build
add_library(stb_image STATIC ${STB_IMAGE_SOURCE})
target_include_directories(stb_image PUBLIC ${STB_INCLUDE})
add_library(stb_vorbis STATIC ${STB_VORBIS_SOURCE})
target_include_directories(stb_vorbis PUBLIC ${STB_INCLUDE})
