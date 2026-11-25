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
Copy-Item -LiteralPath (Join-Path $PSScriptRoot "build\RecoveryCopy.exe") -Destination $dest
$data=Join-Path $dest 'data'
New-Item -ItemType Directory -Path $data | Out-Null
& icacls.exe $data /inheritance:r /grant:r '*S-1-5-18:(OI)(CI)F' '*S-1-5-32-544:(OI)(CI)F' '*S-1-5-19:(OI)(CI)M'
if ($LASTEXITCODE) { throw 'Data ACL setup failed' }
$installed=Join-Path $dest 'LocalGuard.exe'
(Get-FileHash -LiteralPath $installed -Algorithm SHA256).Hash.ToLowerInvariant() | Set-Content -LiteralPath (Join-Path $data 'binary.sha256') -Encoding ascii
@('LocalGuard.exe','RecoveryCopy.exe') | ForEach-Object { (Get-FileHash -LiteralPath (Join-Path $dest $_) -Algorithm SHA256).Hash.ToLowerInvariant()+' '+$_ } | Set-Content -LiteralPath (Join-Path $data 'components.sha256') -Encoding ascii
'LocalGuard baseline' | Set-Content -LiteralPath (Join-Path $data 'watch.txt') -Encoding ascii
New-Service -Name LocalGuard -BinaryPathName ('"'+$installed+'" --service') -StartupType Manual -Description 'LocalGuard defensive observation' | Out-Null
& sc.exe config LocalGuard obj= 'NT AUTHORITY\LocalService' password= ''
if ($LASTEXITCODE) { throw 'Account configuration failed; remove service before retrying' }
& sc.exe failure LocalGuard reset= 86400 actions= 'restart/60000/restart/300000/none/0'
if ($LASTEXITCODE) { throw 'Recovery configuration failed; inspect service before starting' }
& sc.exe failureflag LocalGuard 1
if ($LASTEXITCODE) { throw 'Failure flag configuration failed; inspect service before starting' }
& sc.exe qfailure LocalGuard
if ($LASTEXITCODE) { throw 'Recovery query failed' }
Write-Host 'Installed in manual mode. Start-Service LocalGuard to validate. No automatic scans in service mode.'
