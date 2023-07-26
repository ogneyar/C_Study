echo off
cls
make clean
make
if "%errorlevel%" == "0" (goto :1)
pause
:1