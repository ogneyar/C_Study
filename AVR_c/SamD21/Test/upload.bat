@echo off
setlocal

rem get last COM port
for /f "delims=" %%I in ('wmic path Win32_SerialPort get DeviceID^,Caption^,Description^,Name^,ProviderType /format:list ^| find "="') do (
    set "%%I"
)

echo Using %DeviceID% for upload
echo Executing "bossac --info --port "%DeviceID%" --write --verify --reset --erase -U true main.bin"
@REM bossac --info --port "%DeviceID%" --write --verify --reset --erase -U false main.bin
C:\Users\ogney\AppData\Local\Arduino15\packages\arduino\tools\bossac\1.7.0-arduino3\bossac.exe --info --port "%DeviceID%" --write --verify --reset --erase -U false main.bin
@REM bossac --info --port "%DeviceID%" --verify main.bin

if %ERRORLEVEL% EQU 0 (
   echo Firmware uploaded successfully!
) else (
   echo Error uploading firmware! Code: %errorlevel%
   exit /b %errorlevel%
)

pause