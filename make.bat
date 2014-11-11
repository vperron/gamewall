@echo off

set name=GameWall
set oname=GWall

if exist %name%.exe del %name%.exe
if exist %name%.obj del %name%.obj
if exist rsrc.obj   del rsrc.obj

cd ExtDll
echo Compiling the dll...
echo.
call BUILD.BAT
cd..
echo.
echo **************          Dll compiling OK            ****************
echo.
cd Rkmd
echo Compiling the Uninstall Exe...
echo.
call make.bat
cd..
echo.
echo **************    Uninstall Add-on compiling OK     ****************
echo.
cd KMD
echo Compiling the Driver...
echo.
call MakeKMD.bat
cd..
echo.
echo **************   Kernel Mode Driver compiling OK    ****************
echo.
cd ServProg
echo Compiling the Server Program...
echo.
call make.bat
cd..
echo.
echo **************     Server Program compiling OK      ****************
echo.
echo Compiling the executable...
echo.
\masm32\bin\ml /c /nologo /coff %name%.asm
\masm32\bin\rc rsrc.rc
\masm32\bin\cvtres /nologo /machine:ix86 rsrc.res
\masm32\bin\Link /nologo /SECTION:.text,ERW /SUBSYSTEM:WINDOWS %name%.obj rsrc.obj /out:%oname%.exe
echo.
echo **************             Exe compiling OK         ****************
echo.
if exist %name%.obj del %name%.obj
if exist rsrc.obj del rsrc.obj
if exist *.bak del *.bak

copy /Y %oname%.exe ReadyToUse\%oname%.exe
copy /Y %kname%.sys ReadyToUse\%kname%.sys
copy /Y ReadMe.txt ReadyToUse\ReadMe.txt
copy /Y Rkmd\%nam%.exe ReadyToUse\%nam%.exe
copy /Y ServProg\%Snam%.exe ReadyToUse\%Snam%.exe

rem fsg.exe %oname%.exe

echo.
echo.
echo Compilation successfully completed.
echo.
pause