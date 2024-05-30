@echo off

rem The CHM tool must be on path!
set CURRENT_PATH=%~dp0
set OLD_PATH=%PATH%
set PATH=%PATH%;%CURRENT_PATH%ChmTools

rem Generate
cd ..\Coding\
doxygen 110012_00992_00882_DOC-Doxyfile | ..\Design\GnuTools\egrep --color=always "warning:|error"
cd ..\Design\

rem Restore previous path variable
set PATH=%OLD_PATH%
pause