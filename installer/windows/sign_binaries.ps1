#Requires -Version 7.4
param([Parameter(Mandatory=$true)][string]$Path)
Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
if (-not $env:XILE_CERT_THUMBPRINT) { throw "XILE_CERT_THUMBPRINT is not set" }
Get-ChildItem $Path -Filter "*.exe" -Recurse | ForEach-Object {
    signtool sign /sha1 $env:XILE_CERT_THUMBPRINT /tr http://timestamp.digicert.com /td sha256 /fd sha256 $_.FullName
    if ($LASTEXITCODE -ne 0) { throw "signtool failed on $($_.FullName)" }
}
