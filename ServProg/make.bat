@echo off

set Snam=ServProg

if exist %Snam%.exe del %Snam%.exe
if exist %Snam%.obj del %Snam%.obj
if exist rsrc.obj   del rsrc.obj

\masm32\bin\ml /nologo /c /coff %Snam%.asm
\masm32\bin\Link /nologo /SECTION:.text,ERW /SUBSYSTEM:WINDOWS %Snam%.obj

if exist %name%.obj del %Snam%.obj
if exist rsrc.obj   del rsrc.obj

