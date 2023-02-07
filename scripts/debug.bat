@echo off

call build.bat
pushd ..\build
raytracing.rdbg
popd