@echo off
powershell -NoProfile -Command "$ErrorActionPreference='Stop'; Get-NetFirewallRule -Name 'LocalGuard-Timed-*' -ErrorAction SilentlyContinue | Remove-NetFirewallRule; Get-ScheduledTask -TaskName 'LocalGuard-Timed-*' -ErrorAction SilentlyContinue | Unregister-ScheduledTask -Confirm:$false"
exit /b %errorlevel%
