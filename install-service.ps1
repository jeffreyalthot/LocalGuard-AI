#Requires -RunAsAdministrator
$ErrorActionPreference='Stop'
$exe=Join-Path $PSScriptRoot 'build\LocalGuard.exe'
if (!(Test-Path -LiteralPath $exe -PathType Leaf)) { throw 'Build first' }
if (Get-Service LocalGuard -ErrorAction SilentlyContinue) { throw 'Service already exists' }
# Do not run privileged code from a user-writable build tree.
$dest=Join-Path $env:ProgramFiles 'LocalGuard'
if (Test-Path -LiteralPath $dest) { throw 'Destination already exists; inspect manually' }
if ((Read-Host 'Install verified binary as LocalService? Type INSTALL') -cne 'INSTALL') { return }
New-Item -ItemType Directory -Path $dest | Out-Null
Copy-Item -LiteralPath $exe -Destination $dest
$data=Join-Path $dest 'data'
New-Item -ItemType Directory -Path $data | Out-Null
& icacls.exe $data /inheritance:r /grant:r '*S-1-5-18:(OI)(CI)F' '*S-1-5-32-544:(OI)(CI)F' '*S-1-5-19:(OI)(CI)M'
if ($LASTEXITCODE) { throw 'Data ACL setup failed' }
$installed=Join-Path $dest 'LocalGuard.exe'
(Get-FileHash -LiteralPath $installed -Algorithm SHA256).Hash.ToLowerInvariant() | Set-Content -LiteralPath (Join-Path $data 'binary.sha256') -Encoding ascii
'LocalGuard baseline' | Set-Content -LiteralPath (Join-Path $data 'watch.txt') -Encoding ascii
New-Service -Name LocalGuard -BinaryPathName ('"'+$installed+'" --service') -StartupType Manual -Description 'LocalGuard defensive observation' | Out-Null
& sc.exe config LocalGuard obj= 'NT AUTHORITY\LocalService' password= ''
if ($LASTEXITCODE) { throw 'Account configuration failed; remove service before retrying' }
Write-Host 'Installed in manual mode. Start-Service LocalGuard to validate. No automatic scans in service mode.'
