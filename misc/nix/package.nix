{ lib, stdenv, cmake, ninja, fetchFromGitHub, xorg, xorgserver, pkg-config
, SDL2, openal, libGLX, libvorbis, stb, clang }:

stdenv.mkDerivation (finalAttrs: {
  pname = "nbcraft";
  version = "0.1.0";

  src = ../../.;

  nativeBuildInputs = [ cmake ninja clang ];
  buildInputs = [ SDL2 openal libGLX libvorbis
    # Build a newer version of stb 
    (stb.overrideAttrs (previousAttrs: {
      version = "0-unstable-2026-04-15";
      src = fetchFromGitHub {
        owner = "nothings";
        repo = "stb";
        rev = "31c1ad37456438565541f4919958214b6e762fb4";
        hash = "sha256-m2yNUlA37hDkKQVrQ+R8nufHfW/cXLnMo+n1X1Cyun0=";
      };
    })
    )
  ];

  cmakeFlags = [
    "-D NIX_BUILD=ON"
    "-D CMAKE_C_COMPILER=${clang}/bin/clang"
    "-D CMAKE_CXX_COMPILER=${clang}/bin/clang++"
    "-D CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES=${stb}/include/stb/"
    "-D STB_INCLUDE=${stb}/include/stb/"
    "-D STB_VORBIS_SOURCE=${stb}/include/stb/stb_vorbis.c"
    "-D CMAKE_BUILD_TYPE=Debug"
  ];
})
