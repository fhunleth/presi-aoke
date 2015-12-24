@echo off

echo Compiling...
cd windeployqt
qmake windeployqt.pro
IF ERRORLEVEL ==1 GOTO :ERROR
mingw32-make.exe release
IF ERRORLEVEL ==1 GOTO :ERROR
cd ..

qmake presi-aoke.pro
IF ERRORLEVEL ==1 GOTO :ERROR
mingw32-make.exe release
IF ERRORLEVEL ==1 GOTO :ERROR

echo Copying files to staging
del /Q /S .\staging
mkdir .\staging
copy .\release\presi-aoke.exe .\staging

echo Copying qt libraries to staging
.\windeployqt\release\windeployqt.exe .\staging\presi-aoke.exe -no-translations -no-webkit2 -opengl

echo Creating installer

if exist "C:\Program Files (x86)\NSIS\makensis.exe" (set nsis="C:\Program Files (x86)\NSIS\makensis.exe") else (set nsis="C:\Program Files\NSIS\makensis.exe")
%nsis% presi-aoke.nsi
IF ERRORLEVEL ==1 GOTO :ERROR

echo Build complete
EXIT /B 0

:ERROR
ECHO Build failed.
EXIT /B 1
