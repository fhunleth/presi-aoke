!define BUILD_PRODUCTS_BASE "./staging"
!define VERSION 0.8.0.0

!include MUI2.nsh
!include Library.nsh

Name "Presi-aoki ${VERSION}"

; The file to write
OutFile "presi-aoki-${VERSION}.exe"
SetCompressor lzma

; Branding
;;;!define MUI_ICON eye.ico
BrandingText "Frank Hunleth"

; The default installation directory
InstallDir $PROGRAMFILES\FrankHunleth\Presi-aoki

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\FrankHunleth\Presi-aoki" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

;!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\nsis.bmp" ; optional
!define MUI_ABORTWARNING

;Page components
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
; Languages
!insertmacro MUI_LANGUAGE "English"

; The stuff to install
Section "Main (required)"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\bin

  File /r ${BUILD_PRODUCTS_BASE}\*

  SetOutPath $INSTDIR

  ; Write the installation path into the registry
  WriteRegStr HKLM "Software\FrankHunleth\Presi-aoki" "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoki" "DisplayName" "VR Stripes"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoki" "Publisher" "Frank Hunleth"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoki" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoki" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoki" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoki" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

Section "Start Menu Shortcuts"
  SetShellVarContext all

  CreateShortCut "$SMPROGRAMS\Presi-aoki.lnk" "$INSTDIR\bin\presi-aoki.exe" "" "$INSTDIR\bin\presi-aoki.exe" 0
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoki"
  DeleteRegKey HKLM "Software\FrankHunleth\Presi-aoki"

  ; Remove files
  RMDir /r $INSTDIR\bin

  ; Uninstaller
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  SetShellVarContext all
  Delete "$SMPROGRAMS\Presi-aoki.lnk"

  ; Remove directories used
  RMDir "$INSTDIR"

SectionEnd


;--------------------------------
;Version Information

VIProductVersion "${VERSION}.0"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "Presi-aoki"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Frank Hunleth"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright (C) 2015"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Setup Application"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${VERSION}"
