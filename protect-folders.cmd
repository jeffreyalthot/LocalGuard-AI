@echo off
 echo Audit mode only: records potential Controlled Folder Access blocks without blocking.
choice /M "Enable Defender Controlled Folder Access audit mode"
if errorlevel 2 exit /b 2
powershell -NoProfile -Command "$ErrorActionPreference='Stop'; $p=Get-MpPreference; if($p.EnableControlledFolderAccess -eq 0){Set-MpPreference -EnableControlledFolderAccess AuditMode}; Get-MpPreference | Select-Object EnableControlledFolderAccess"
exit /b %errorlevel%
