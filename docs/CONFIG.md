# xile.ini Reference

All keys are flat. Section headers are accepted and ignored.

| Key | Type | Default | Description |
| --- | --- | --- | --- |
| `mode` | enum | `overlay` | `overlay` or `shell_replace`. |
| `autostart` | bool | `false` | Starts Xile at login when enabled. |
| `display_number` | string | `:1` | X display number. |
| `wm` | enum | `icewm` | Window manager. Xile 1.0 supports IceWM. |
| `icewm_theme` | string | `Xile` | IceWM theme name. |
| `exit_chord` | string | `ctrl+alt+shift+f12` | Overlay exit chord. |
| `log_level` | enum | `info` | `debug`, `info`, `warn`, or `error`. |
| `render_backend` | enum | `auto` | `auto`, `gdi`, `d3d11`, `metal`, or `cgl`. |

Search order:

1. Windows: `C:\ProgramData\Xile\xile.ini`; macOS: `/Library/Application Support/Xile/xile.ini`
2. Windows: `%APPDATA%\Xile\xile.ini`; macOS: `~/Library/Application Support/Xile/xile.ini`
3. CLI: `--config <path>`
