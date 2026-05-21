Section "Uninstall"
  ExecWait '"$INSTDIR\xile-ctl.exe" stop'
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "Xile"
  RMDir /r "$INSTDIR"
SectionEnd
