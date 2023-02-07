@echo off

pushd ..\build\cpp
raytracing_cpp.exe >> image_cpp.ppm
popd