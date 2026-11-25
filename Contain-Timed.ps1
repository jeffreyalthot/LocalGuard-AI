$ErrorActionPreference = 'Stop'
$principal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'Run as administrator' }
$targetId = 0
if (-not [int]::TryParse((Read-Host 'Investigated process PID'), [ref]$targetId) -or $targetId -le 4 -or $targetId -eq $PID) { throw 'Invalid PID' }
$p = Get-Process -Id $targetId
$path = $p.Path
$started = $p.StartTime
if (-not $path) { throw 'Process path unavailable' }
$item = Get-Item -LiteralPath $path
if ($item.PSIsContainer -or $item.Extension -ne '.exe' -or $item.FullName -notmatch '^[A-Za-z]:\\' ) { throw 'Invalid executable' }
$path = $item.FullName
# Restrict to local fixed drives, reject reparse points throughout path.
$drive = New-Object IO.DriveInfo($item.PSDrive.Root)
if ($drive.DriveType -ne [IO.DriveType]::Fixed) { throw 'Not a fixed drive' }
$node = $item
while ($null -ne $node) { if ($node.Attributes -band [IO.FileAttributes]::ReparsePoint) { throw 'Reparse point rejected' }; if ($node -is [IO.FileInfo]) { $node=$node.Directory } else { $node=$node.Parent } }
foreach ($root in @($env:windir,$env:ProgramFiles,${env:ProgramFiles(x86)},$PSHOME,$PSScriptRoot)) { if ($root -and $path.StartsWith($root.TrimEnd('\')+'\',[StringComparison]::OrdinalIgnoreCase)) { throw 'Protected location' } }
if ((Get-AuthenticodeSignature -LiteralPath $path).Status -eq 'Valid') { throw 'Signed executable: use manual investigation' }
$hash = (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash
Write-Host "PID=$targetId Path=$path SHA256=$hash"
$minutes=0
if (-not [int]::TryParse((Read-Host 'Duration in minutes (1-15)'),[ref]$minutes) -or $minutes -lt 1 -or $minutes -gt 15) { throw 'Invalid duration' }
if ((Read-Host 'Confirm independently investigated threat; blocks ALL instances of this executable. Type BLOCK') -cne 'BLOCK') { exit 2 }
$q=Get-Process -Id $targetId
if ($q.StartTime -ne $started -or $q.Path -ne $path -or (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash -ne $hash) { throw 'Identity changed' }
$name='LocalGuard-Timed-'+[guid]::NewGuid().ToString('N')
# Register cleanup before creating the firewall rule. No user-controlled text in action.
$action=New-ScheduledTaskAction -Execute "$env:SystemRoot\System32\WindowsPowerShell\v1.0\powershell.exe" -Argument ('-NoProfile -NonInteractive -Command "Remove-NetFirewallRule -Name '+$name+' -ErrorAction SilentlyContinue"')
$trigger=New-ScheduledTaskTrigger -Once -At (Get-Date).AddMinutes($minutes)
$settings=New-ScheduledTaskSettingsSet -StartWhenAvailable -AllowStartIfOnBatteries -DontStopIfGoingOnBatteries
Register-ScheduledTask -TaskName $name -Action $action -Trigger $trigger -Settings $settings -User SYSTEM -RunLevel Highest | Out-Null
try {
 New-NetFirewallRule -Name $name -DisplayName $name -Direction Outbound -Action Block -Program $path -Profile Any -Enabled True | Out-Null
 Get-NetFirewallRule -Name $name -PolicyStore ActiveStore | Format-List Name,Enabled,Action,Direction
 Write-Host "Cleanup scheduled in $minutes minutes. Emergency: release-timed.cmd"
} catch { Remove-NetFirewallRule -Name $name -ErrorAction SilentlyContinue; throw }
