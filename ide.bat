@echo off
:Start
echo.
echo.
echo -------------
echo (1) Make
echo (2) Build
echo (3) Build + Run
echo (4) Rebuild
echo (5) Make+Rebuild
echo (6) Run
echo.

choice /N /C 123456

if %errorlevel%==1 goto Make
if %errorlevel%==2 goto Build
if %errorlevel%==3 goto BuildRun
if %errorlevel%==4 goto Rebuild
if %errorlevel%==5 goto MakeRebuild
if %errorlevel%==6 goto Run

goto Start

:Make
call make.bat
goto Start

:Build
call build.bat
goto Start

:BuildRun
call build.bat
echo Build finished: %errorlevel%
if %errorlevel%==0 goto Run
goto Start

:Rebuild
call build.bat Rebuild
goto Start

:MakeRebuild
call make.bat
call build.bat Rebuild
goto Start

:Run
bin\Game\Game_Debug.exe
goto Start