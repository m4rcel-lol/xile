# Building Xile

## Requirements

- CMake 3.27 or newer
- Ninja
- Windows: MSVC 19.38 or newer, PowerShell 7.4
- macOS: Xcode command line tools, macOS 12 SDK or newer

## Configure

```sh
cmake --preset windows-x64
cmake --preset windows-arm64
cmake --preset macos-x86_64
cmake --preset macos-arm64
```

## Build

```sh
cmake --build --preset windows-x64 --parallel
cmake --build --preset macos-arm64 --parallel
```

## Installer CI

The workflow `.github/workflows/build-installers.yml` builds and uploads:

- Windows x64 NSIS installer
- Windows ARM64 NSIS installer
- macOS x86_64 pkg
- macOS arm64 pkg
- macOS universal pkg

Run it manually from GitHub Actions with `workflow_dispatch`, or let it run on pushes and pull requests.

## Third-Party Sources

Populate `third_party/` with these exact releases before enabling full Xorg/IceWM source builds:

- xorg-server 21.1.11
- IceWM 3.4.5
- pixman 0.42.2
- libxcb 1.16
- xcb-proto 1.16
- libX11 1.8.7
- libXext 1.3.5
- libXrender 0.9.11
- libXft 2.3.8
- FreeType 2.13.2
- fontconfig 2.15.0
- OpenSSL 3.3.0
- munit commit `fbbdf1467`
