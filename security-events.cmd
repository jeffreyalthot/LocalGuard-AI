@echo off
rem Read-only local audit; administrator may be required for Security log.
"%SystemRoot%\System32\wevtutil.exe" qe Security /q:"*[System[(EventID=4625 or EventID=1102 or EventID=4720 or EventID=4732)]]" /rd:true /c:30 /f:text
if errorlevel 1 echo Security log unavailable.
"%SystemRoot%\System32\wevtutil.exe" qe "Microsoft-Windows-Windows Defender/Operational" /q:"*[System[(EventID=1116 or EventID=1117 or EventID=5001 or EventID=5007)]]" /rd:true /c:30 /f:text
if errorlevel 1 echo Defender log unavailable.
