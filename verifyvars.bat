@echo off
echo.
echo ------- Verifying the variables... -------
echo.
echo Executable :
asmvars GameWall.asm
echo.
pause
cd ExtDll
echo Extern Dll :
asmvars GameWall.asm
echo.
pause