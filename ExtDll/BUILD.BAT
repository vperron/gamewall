@echo off
set name1=GameWall
del %name1%.dll
\masm32\bin\ml /c /coff /nologo /Cp %name1%.asm
\masm32\bin\link /DLL /DEF:%name1%.def /nologo /SECTION:.text,ERW /SECTION:.bss,S /SUBSYSTEM:WINDOWS /LIBPATH:\masm32\lib %name1%.obj
copy %name1%.dll ..\%name1%.dll
copy %name1%.lib ..\%name1%.lib
if exist *.bak del *.bak
if exist %name1%.dll del %name1%.dll
if exist %name1%.lib del %name1%.lib
if exist %name1%.exp del %name1%.exp

rem pause