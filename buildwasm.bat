@echo off
call emcc -o game.html src/main.cpp -Os -Wall lib/raylib/libraylib.a -I. -Ivendor/raylib -L. -Llib/raylib -s USE_GLFW=3 -s ASYNCIFY --shell-file minshell.html -DPLATFORM_WEB
if %errorlevel% neq 0 (
    echo Build failed
    exit /b %errorlevel%
)
echo Build succeeded
python -m http.server