#Requires -RunAsAdministrator
$ErrorActionPreference='Stop'
if ((Read-Host 'Remove LocalGuard service? Type REMOVE') -cne 'REMOVE') { return }
$s=Get-Service LocalGuard -ErrorAction Stop
if ($s.Status -ne 'Stopped') { Stop-Service LocalGuard -ErrorAction Stop; $s.WaitForStatus('Stopped',[TimeSpan]::FromSeconds(30)) }
& sc.exe delete LocalGuard
if ($LASTEXITCODE) { throw 'Service deletion failed' }
Write-Host 'Service removed; installed files and evidence retained.'
