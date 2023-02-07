@echo off

call build.bat
pushd ..\build
raytracingclean.rdbg
popd