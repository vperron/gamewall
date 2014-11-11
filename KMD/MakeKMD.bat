@echo off
set kname=GWallDev

if exist %name%.sys del %name%.sys
if exist *.bak del *.bak

\masm32\bin\ml /c /coff /Gz /Cp /nologo %kname%.asm
\masm32\bin\link /nologo /DRIVER /BASE:0X10000 /ALIGN:32 /OUT:%kname%.sys /SUBSYSTEM:NATIVE /IGNORE:4078 /OPTidata %kname%.obj

move %kname%.sys ..

del *.obj

echo.