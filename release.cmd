@echo off
powershell -NoProfile -Command "$ErrorActionPreference='Stop'; Remove-NetFirewallRule -Name LocalGuard-Containment"
exit /b %errorlevel%
