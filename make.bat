@echo off
msbuild /nologo /verbosity:minimal /t:Build /p:Configuration=Release build\CodeGen.csproj
bin\CodeGen\CodeGen.exe src\ obj\.gen\ /C
rmdir /s /q build
rmdir /s /q bin
premake5 --file=premake.lua vs2015