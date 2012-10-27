@echo off
if not exist build mkdir build
cd build
cmake -G "Visual Studio 11 Win64" ..
start wield.sln
cd ..
