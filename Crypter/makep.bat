@echo off

set name=Crypter

if exist %name%.exe del %name%.exe
if exist %name%.obj del %name%.obj
if exist rsrc.obj   del rsrc.obj

\masm32\bin\ml /c /coff %name%.asm
\masm32\bin\Link /SECTION:.text,ERW /SUBSYSTEM:WINDOWS %name%.obj

rem fsg.exe %name%.exe

if exist %name%.obj del %name%.obj
if exist rsrc.obj   del rsrc.obj

echo.
echo.
echo.
echo Compilation terminee !!!

pause