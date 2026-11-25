@echo off
setlocal
 echo WARNING: Blocks outbound traffic for ONE executable using Windows Firewall.
 echo Existing connections may not terminate immediately. Do not select system executables.
set /p "LG_TARGET=Full executable path: "
if not defined LG_TARGET exit /b 1
powershell -NoProfile -Command "$ErrorActionPreference='Stop'; $p=Get-Item -LiteralPath $env:LG_TARGET; if($p.PSIsContainer -or $p.Extension -ne '.exe' -or $p.FullName.StartsWith($env:windir+'\',[StringComparison]::OrdinalIgnoreCase)){throw 'Invalid or system executable'}; if(Get-NetFirewallRule -Name LocalGuard-Containment -ErrorAction SilentlyContinue){throw 'Existing containment rule; undo first'}; if((Read-Host ('Block outbound for '+$p.FullName+'? Type BLOCK')) -cne 'BLOCK'){exit 2}; New-NetFirewallRule -Name LocalGuard-Containment -DisplayName 'LocalGuard manual containment' -Direction Outbound -Action Block -Program $p.FullName -Profile Any -Enabled True | Out-Null; Get-NetFirewallRule -Name LocalGuard-Containment | Format-List Name,Enabled,Direction,Action"
exit /b %errorlevel%
