@echo off
set TARGET=Build
set CONFIG=Debug
set VERBOSITY=minimal

if [%1] NEQ [] (
	set TARGET=%1
)
if [%2] NEQ [] (
	set CONFIG=%2
)

echo -- %TARGET% Game --
msbuild /nologo /verbosity:%VERBOSITY% /t:%TARGET% /p:Configuration=%CONFIG% build\Game.vcxproj

if %errorlevel% NEQ 0 (
	EXIT /B 1
)

EXIT /B 0
echo.
echo -- %TARGET% Server --
msbuild /nologo /verbosity:%VERBOSITY% /t:%TARGET% /p:Configuration=%CONFIG%Server build\Game.vcxproj