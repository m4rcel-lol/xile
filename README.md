# Xile

Your X11 world, on any machine.

Xile is a native C foundation for a self-contained X11 desktop environment on Windows and macOS with IceWM as the default window manager. This repository contains the build system, configuration parser, logger, Xauthority writer, IPC framing, platform overlay backends, control CLI, installer workflows, tests, and recovery documentation.

## Build

```sh
cmake --preset windows-x64
cmake --build --preset windows-x64 --parallel
```

On macOS:

```sh
cmake --preset macos-arm64
cmake --build --preset macos-arm64 --parallel
```

## Installer Workflows

GitHub Actions builds all installer targets in `.github/workflows/build-installers.yml`:

- Windows x64: `installer/windows/Xile-Setup-x64.exe`
- Windows ARM64: `installer/windows/Xile-Setup-arm64.exe`
- macOS x86_64: `dist/macos/Xile-1.0.0-x86_64.pkg`
- macOS arm64: `dist/macos/Xile-1.0.0-arm64.pkg`
- macOS universal: `dist/macos/Xile-1.0.0-universal.pkg`

## Usage

```sh
xile-server --config path/to/xile.ini
xile-ctl status
xile-ctl stop
xile-ctl restart-wm
xile-ctl autostart enable
xile-ctl mode overlay
```

The overlay exit chord is `Ctrl+Alt+Shift+F12`.

## Recovery

Windows shell recovery:

```cmd
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v Shell /t REG_SZ /d explorer.exe /f
```

macOS recovery:

```sh
xile-recover
```

More detail is in `docs/RECOVERY.md`.

## Documentation

- `docs/ARCHITECTURE.md`
- `docs/BUILD.md`
- `docs/CONFIG.md`
- `docs/RECOVERY.md`

## License

MIT
