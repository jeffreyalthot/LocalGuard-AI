@echo off
cd /d "%~dp0"
if not exist build\LocalGuard.exe (call build.cmd || exit /b 1)
choice /M "Allow automatic Microsoft Defender quick scans on repeated urgent alerts"
if errorlevel 2 exit /b 2
build\LocalGuard.exe --auto-scan
