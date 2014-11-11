@echo off
d:\x\tasm32\bin\brc32 -r raw2db.rc
d:\x\tasm32\bin\tasm32 -ml -m5 -q raw2db
d:\x\tasm32\bin\tlink32 -Tpe -aa -c -x -V4.0 raw2db.obj,..\raw2db.exe,,import32,,raw2db.res

del raw2db.obj
del raw2db.res
