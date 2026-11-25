@echo off
cd /d "%~dp0"
echo Only provision immediately after a trusted build. Never rebaseline a suspicious binary.
choice /M "Trust this freshly built executable"
if errorlevel 2 exit /b 1
powershell -NoProfile -Command "$ErrorActionPreference='Stop'; $p='build\data\binary.sha256'; if(Test-Path -LiteralPath $p){throw 'Existing baseline: preserve and investigate before replacing manually'}; New-Item -ItemType Directory -Force -Path 'build\data' | Out-Null; $h=(Get-FileHash -LiteralPath 'build\LocalGuard.exe' -Algorithm SHA256).Hash.ToLowerInvariant(); $f=[IO.File]::Open($p,[IO.FileMode]::CreateNew); try {$b=[Text.Encoding]::ASCII.GetBytes($h); $f.Write($b,0,$b.Length)} finally {$f.Dispose()}"
exit /b %errorlevel%
