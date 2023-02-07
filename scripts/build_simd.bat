@echo off

SETLOCAL
set code_path=..\..\code\
Rem set import_libraries

mkdir ..\build\simd
pushd ..\build\simd
cl /Zi /WX /arch:AVX2 ..\..\code\simd_version\raytracing_simd.cpp
popd
ENDLOCAL