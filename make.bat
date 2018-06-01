@echo off
msbuild /nologo /verbosity:minimal /t:Build /p:Configuration=Release /p:Platform=x64 Build\CodeGen.csproj
bin\CodeGen\CodeGen.exe Source\ Build\gen\ /G /V
premake5 --file=premake.lua vs2017