@echo off

set nam=Rkmd

if exist %nam%.exe del %nam%.exe
if exist %nam%.obj del %nam%.obj
if exist rsrc.obj   del rsrc.obj

\masm32\bin\ml /c /nologo /coff %nam%.asm
rem \masm32\bin\rc rsrc.rc
rem \masm32\bin\cvtres /nologo /machine:ix86 rsrc.res
\masm32\bin\link /nologo /SUBSYSTEM:WINDOWS %nam%.obj
if exist %nam%.obj del %nam%.obj
if exist rsrc.obj del rsrc.obj
if exist *.bak del *.bak
