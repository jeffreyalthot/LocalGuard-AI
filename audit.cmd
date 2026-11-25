@echo off
powershell -NoProfile -Command "$ErrorActionPreference='Stop'; Get-MpComputerStatus | Select-Object AntivirusEnabled,RealTimeProtectionEnabled,BehaviorMonitorEnabled,IsTamperProtected,AntivirusSignatureLastUpdated; Get-NetFirewallProfile | Select-Object Name,Enabled,DefaultInboundAction,DefaultOutboundAction; Get-MpPreference | Select-Object EnableControlledFolderAccess,PUAProtection"
exit /b %errorlevel%
