@echo off

SETLOCAL
set code_path=..\..\code\
Rem set import_libraries

mkdir ..\build\cpp
pushd ..\build\cpp
cl /Zi /WX /EHsc /arch:AVX2  ..\..\code\cpp_version\raytracing_cpp.cpp
popd
ENDLOCAL