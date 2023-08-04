echo off
cls
echo ===
echo ========
make
if "%errorlevel%" == "0" (goto :1)
pause
:1