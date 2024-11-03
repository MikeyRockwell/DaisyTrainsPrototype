@echo off
setlocal enabledelayedexpansion

:: Set up the Emscripten environment
call C:\emsdk\emsdk_env.bat

set SOURCES=
for %%f in (src\*.cpp) do (
    set SOURCES=!SOURCES! %%f
)

call emcc -o game.html !SOURCES! -O3 -Wall -I. -Ivendor/raylib -L. -Llib/raylib lib/raylib/libraylib.a -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 --shell-file minshell.html -DPLATFORM_WEB --preload-file res
if %errorlevel% neq 0 (
    echo Build failed
    exit /b %errorlevel%
)
echo Build succeeded
python -m http.server