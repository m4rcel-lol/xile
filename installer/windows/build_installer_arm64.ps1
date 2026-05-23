#Requires -Version 7.4
Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path

$requiredTools = @("cmake", "ninja", "makensis")
foreach ($tool in $requiredTools) {
    if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) {
        throw "Required tool not found: $tool"
    }
}

Push-Location $repoRoot
try {
    cmake --preset windows-arm64
    cmake --build --preset windows-arm64 --parallel

    $buildDir = "build\windows-arm64"
    $distDir = "dist\windows-arm64"
    if (Test-Path $distDir) { Remove-Item -Recurse -Force $distDir }
    New-Item -ItemType Directory -Force -Path $distDir | Out-Null
    Copy-Item "$buildDir\xile-server.exe" $distDir -Force
    Copy-Item "$buildDir\xile-ctl.exe" $distDir -Force
    New-Item -ItemType Directory -Force -Path "$distDir\themes" | Out-Null
    Copy-Item -Recurse "src\fonts" "$distDir\fonts" -Force
    Copy-Item -Recurse "src\wm\icewm_theme\Xile" "$distDir\themes\Xile" -Force

    if ($env:XILE_CERT_THUMBPRINT -and (Get-Command signtool -ErrorAction SilentlyContinue)) {
        Get-ChildItem $distDir -Filter "*.exe" -Recurse | ForEach-Object {
            signtool sign /sha1 $env:XILE_CERT_THUMBPRINT /tr http://timestamp.digicert.com /td sha256 /fd sha256 $_.FullName
            if ($LASTEXITCODE -ne 0) { throw "signtool failed on $($_.FullName)" }
        }
    }

    makensis /DARCH=arm64 /DPROJECT_DIR="$repoRoot" /DDIST_DIR="$((Resolve-Path $distDir).Path)" installer\windows\xile_installer_arm64.nsi
    if ($LASTEXITCODE -ne 0) { throw "NSIS build failed" }
} finally {
    Pop-Location
}
