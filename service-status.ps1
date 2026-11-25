$ErrorActionPreference='Stop'
Get-Service -Name LocalGuard | Format-List Name,Status,StartType
& sc.exe qfailure LocalGuard
if ($LASTEXITCODE) { throw 'Recovery query failed' }
& sc.exe qfailureflag LocalGuard
if ($LASTEXITCODE) { throw 'Failure flag query failed' }
