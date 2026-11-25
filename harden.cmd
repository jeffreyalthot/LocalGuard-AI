@echo off
 echo Enables Windows Firewall on all profiles and requests Defender PUA protection.
 echo Run as administrator. Organization policies may override these settings.
choice /M "Apply these settings"
if errorlevel 2 exit /b 2
powershell -NoProfile -Command "$ErrorActionPreference='Stop'; Set-NetFirewallProfile -Profile Domain,Private,Public -Enabled True; Get-NetFirewallProfile | Format-Table Name,Enabled; Set-MpPreference -PUAProtection Enabled; Get-MpPreference | Select-Object PUAProtection"
exit /b %errorlevel%
