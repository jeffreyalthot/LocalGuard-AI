@echo off
setlocal
set "LOCALGUARD_WATCH_DIR="
set /p "LOCALGUARD_WATCH_DIR=Full path of folder to monitor: "
if not defined LOCALGUARD_WATCH_DIR exit /b 1
call "%~dp0run.cmd"
