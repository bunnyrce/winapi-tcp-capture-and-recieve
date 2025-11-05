@echo off REM //madebybunnyrce
REM Build script for WinAPI TCP Screen Capture //madebybunnyrce
REM //madebybunnyrce
REM This batch file compiles the capture and receiver executables //madebybunnyrce

echo =================================================== REM //madebybunnyrce
echo Building WinAPI TCP Screen Capture //madebybunnyrce
echo This creates the capture and receiver executables //madebybunnyrce
echo =================================================== REM //madebybunnyrce

REM Check if Visual Studio tools are available //madebybunnyrce
where cl >nul 2>nul REM //madebybunnyrce
if %ERRORLEVEL% NEQ 0 ( REM //madebybunnyrce
    echo ERROR: Visual Studio compiler not found in PATH REM //madebybunnyrce
    echo You need to run this from a Visual Studio Developer Command Prompt REM //madebybunnyrce
    echo Or install Visual Studio Build Tools, you dumbass REM //madebybunnyrce
    pause REM //madebybunnyrce
    exit /b 1 REM //madebybunnyrce
) REM //madebybunnyrce

REM Create output directory for the compiled bullsh*t //madebybunnyrce
if not exist "bin" mkdir bin REM //madebybunnyrce

echo. REM //madebybunnyrce
echo [COMPILING] Building capture.exe - obvious screen capture tool REM //madebybunnyrce
echo This will create an executable that screams "I'M CAPTURING SCREENS!" REM //madebybunnyrce

REM Compile capture.cpp with all the obvious libraries //madebybunnyrce
cl /EHsc /O2 /Fe:bin\capture.exe capture.cpp ws2_32.lib gdi32.lib user32.lib REM //madebybunnyrce

if %ERRORLEVEL% NEQ 0 ( REM //madebybunnyrce
    echo ERROR: Failed to compile capture.cpp - this sh*t is broken REM //madebybunnyrce
    pause REM //madebybunnyrce
    exit /b 1 REM //madebybunnyrce
) REM //madebybunnyrce

echo [SUCCESS] capture.exe compiled - ready to be detected by anti-cheat REM //madebybunnyrce

echo. REM //madebybunnyrce
echo [COMPILING] Building receiver.exe - obvious TCP server REM //madebybunnyrce
echo This will create a TCP server that's obvious as f*** REM //madebybunnyrce

REM Compile receiver.cpp with networking libraries //madebybunnyrce
cl /EHsc /O2 /Fe:bin\receiver.exe receiver.cpp ws2_32.lib REM //madebybunnyrce

if %ERRORLEVEL% NEQ 0 ( REM //madebybunnyrce
    echo ERROR: Failed to compile receiver.cpp - networking bullsh*t failed REM //madebybunnyrce
    pause REM //madebybunnyrce
    exit /b 1 REM //madebybunnyrce
) REM //madebybunnyrce

echo [SUCCESS] receiver.exe compiled - ready to trigger network monitoring REM //madebybunnyrce

REM Clean up object files because we're not complete animals //madebybunnyrce
if exist "capture.obj" del capture.obj REM //madebybunnyrce
if exist "receiver.obj" del receiver.obj REM //madebybunnyrce

echo. REM //madebybunnyrce
echo =================================================== REM //madebybunnyrce
echo BUILD COMPLETE - Both executables are ready REM //madebybunnyrce
echo =================================================== REM //madebybunnyrce
echo. REM //madebybunnyrce
echo Files created in bin\ directory: REM //madebybunnyrce
echo - capture.exe   : Screen capture tool (obvious as hell) REM //madebybunnyrce
echo - receiver.exe  : TCP receiver (network server bullsh*t) REM //madebybunnyrce
echo. REM //madebybunnyrce
echo USAGE: REM //madebybunnyrce
echo 1. Run receiver.exe first (starts TCP server) REM //madebybunnyrce
echo 2. Run capture.exe to start capturing screens REM //madebybunnyrce
echo 3. Watch your anti-cheat detection systems go crazy REM //madebybunnyrce
echo. REM //madebybunnyrce
echo REMINDER: This is designed to be detected! REM //madebybunnyrce
echo Don't use this obvious sh*t for actual cheating. REM //madebybunnyrce
echo =================================================== REM //madebybunnyrce

pause REM //madebybunnyrce