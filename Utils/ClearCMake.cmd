@echo off

pushd %~dp0\..

del CMakeCache.txt cmake_install.cmake Makefile build.ninja .ninja* H3LandscapeCMake.sln *.filters *.vcxproj
del /S /Q CMakeFiles\
del /S /Q Landscape.build\
del /S /Q Patcher.build\
del /S /Q Win32\ 2> NUL

popd
