; Pass in the VERSION via the commandline: /DVERSION=x.y.z.w

!define BUILD_PRODUCTS_BASE "./staging"
!include MUI2.nsh
!include Library.nsh

Name "Presi-aoke ${VERSION}"

; The file to write
OutFile "presi-aoke-${VERSION}.exe"
SetCompressor lzma

; Branding
;;;!define MUI_ICON eye.ico
BrandingText "Frank Hunleth"

; The default installation directory
InstallDir $PROGRAMFILES\FrankHunleth\Presi-aoke

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\FrankHunleth\Presi-aoke" "Install_Dir"

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

  DetailPrint ${VERSION}

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\bin

  File /r ${BUILD_PRODUCTS_BASE}\*

  SetOutPath $INSTDIR

  ; Write the installation path into the registry
  WriteRegStr HKLM "Software\FrankHunleth\Presi-aoke" "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoke" "DisplayName" "Presi-aoke"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoke" "Publisher" "Frank Hunleth"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoke" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoke" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoke" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoke" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

Section "Start Menu Shortcuts"
  SetShellVarContext all

  CreateShortCut "$SMPROGRAMS\Presi-aoke.lnk" "$INSTDIR\bin\presi-aoke.exe" "" "$INSTDIR\bin\presi-aoke.exe" 0
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Presi-aoke"
  DeleteRegKey HKLM "Software\FrankHunleth\Presi-aoke"

  ; Remove files
  RMDir /r $INSTDIR\bin

  ; Uninstaller
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  SetShellVarContext all
  Delete "$SMPROGRAMS\Presi-aoke.lnk"

  ; Remove directories used
  RMDir "$INSTDIR"

SectionEnd


;--------------------------------
;Version Information

VIProductVersion "${VERSION}.0"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "Presi-aoke"
VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Frank Hunleth"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright (C) 2015"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Setup Application"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${VERSION}"
