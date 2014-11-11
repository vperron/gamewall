@echo off
cd ReadyToUse\Compiler
echo.
echo -------- Compiling the install script... --------
echo.
iscc "..\GameWall.iss"
echo.
echo --------    Compilation successful !!    --------
echo.
echo.
pause