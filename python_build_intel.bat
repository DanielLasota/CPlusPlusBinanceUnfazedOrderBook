@echo off
setlocal

echo Initializing Intel oneAPI environment...
call "C:\Program Files (x86)\Intel\oneAPI\setvars.bat"

cd /d C:\Users\daniel\CLionProjects\CPlusPlusBinanceUnfazedOrderBook

set SKBUILD_CMAKE_BUILD_TYPE=Release

echo Building wheel...
".venv\Scripts\python.exe" -m build
if errorlevel 1 (
    echo BUILD FAILED
    exit /b 1
)

echo BUILD OK
endlocal