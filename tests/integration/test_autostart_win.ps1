#Requires -Version 7.4
Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$runKey = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
New-Item -Path $runKey -Force | Out-Null
New-ItemProperty -Path $runKey -Name "Xile" -Value "`"C:\Program Files\Xile\xile-server.exe`" --autostart" -PropertyType String -Force | Out-Null
if (-not (Get-ItemProperty -Path $runKey -Name "Xile")) { throw "autostart value missing" }
Remove-ItemProperty -Path $runKey -Name "Xile" -ErrorAction SilentlyContinue
