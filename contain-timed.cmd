@echo off
powershell -NoProfile -File "%~dp0Contain-Timed.ps1"
exit /b %errorlevel%
