@echo off
setlocal
 echo Use Windows File History or a trusted backup tool to copy your files to an external drive.
 echo Verify restoration of a sample file, then disconnect the backup drive.
 echo LocalGuard does not copy or delete user files automatically.
 control.exe /name Microsoft.FileHistory
