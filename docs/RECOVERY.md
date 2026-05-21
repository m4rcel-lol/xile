# Xile Recovery

## Windows

If shell replacement prevents access to the desktop:

1. Open Windows Recovery Environment.
2. Start Command Prompt.
3. Restore the shell value:

```cmd
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v Shell /t REG_SZ /d explorer.exe /f
reg delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v Xile /f
```

Log out and log back in.

## macOS

If Xile starts before you can access the desktop, open Terminal via Spotlight and run:

```sh
xile-recover
```

Manual equivalent:

```sh
launchctl unload "$HOME/Library/LaunchAgents/com.xile.server.plist" 2>/dev/null || true
rm -f "$HOME/Library/LaunchAgents/com.xile.server.plist"
```

Then log out and log back in.
