@echo off
cd /d "%~dp0"
if not exist build\LocalGuard.exe (call build.cmd || exit /b 1)
build\LocalGuard.exe
