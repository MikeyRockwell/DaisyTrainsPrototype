@echo off
setlocal enabledelayedexpansion

set SOURCES=
for %%f in (src\*.cpp) do (
    set SOURCES=!SOURCES! %%f
)

call emcc -o game.html !SOURCES! -Os -Wall lib/raylib/libraylib.a -I. -Ivendor/raylib -L. -Llib/raylib -s USE_GLFW=3 -s ASYNCIFY --shell-file minshell.html -DPLATFORM_WEB --preload-file res
if %errorlevel% neq 0 (
    echo Build failed
    exit /b %errorlevel%
)
echo Build succeeded
python -m http.server