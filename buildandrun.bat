@echo off
echo -------- BUILD ---------

call build.bat
if %errorlevel% NEQ 0 (
	EXIT /B %errorlevel%
)

echo -------- DONE ---------
bin\Game\Game_Debug.exe
