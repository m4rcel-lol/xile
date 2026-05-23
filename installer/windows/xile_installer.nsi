Unicode true
!define APPNAME "Xile"
!define APPVERSION "1.0.0"
!define APPID "{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}"
!define INSTALL_DIR "$PROGRAMFILES64\Xile"
!ifndef PROJECT_DIR
!define PROJECT_DIR "."
!endif

RequestExecutionLevel admin
InstallDir "${INSTALL_DIR}"
InstallDirRegKey HKLM "Software\Xile" "InstallDir"
OutFile "${PROJECT_DIR}\installer\windows\Xile-Setup-${ARCH}.exe"
LicenseData "${PROJECT_DIR}\installer\windows\resources\license.rtf"

Page license
Page directory
Page components
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "Xile Server" SecMain
  SectionIn RO
  SetOutPath "$INSTDIR"
  File /r "${DIST_DIR}\*.*"
  WriteRegStr HKLM "Software\Xile" "InstallDir" "$INSTDIR"
  WriteRegStr HKLM "Software\Xile" "Version" "${APPVERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPID}" "DisplayName" "${APPNAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPID}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPID}" "DisplayVersion" "${APPVERSION}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPID}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPID}" "NoRepair" 1
  CreateDirectory "$SMPROGRAMS\Xile"
  CreateShortcut "$SMPROGRAMS\Xile\Xile.lnk" "$INSTDIR\xile-server.exe"
  CreateShortcut "$SMPROGRAMS\Xile\Uninstall Xile.lnk" "$INSTDIR\uninstall.exe"
  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Uninstall"
  ExecWait '"$INSTDIR\xile-ctl.exe" stop'
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "Xile"
  RMDir /r "$INSTDIR"
  RMDir /r "$SMPROGRAMS\Xile"
  DeleteRegKey HKLM "Software\Xile"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPID}"
SectionEnd
