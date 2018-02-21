@echo off
msbuild /nologo /verbosity:minimal /t:Build /p:Configuration=Release Build\CodeGen.csproj
bin\CodeGen\CodeGen.exe src\ obj\.gen\ /C
premake5 --file=premake.lua vs2015