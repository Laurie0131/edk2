@echo off
setlocal EnableDelayedExpansion EnableExtensions
set exitCode=0
set EXE_BUILD_DIR=""
if /i "%~1"=="/?" goto Usage

if "%~1"=="" goto findPath
Set TAG=%~1
if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_%TAG%\X64\CheckPrimeAppUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_%TAG%\X64
goto EndIfTag

:findPath

if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2013x86\X64\CheckPrimeAppUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2013x86\X64
if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2015x86\X64\CheckPrimeAppUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2015x86\X64
if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2017\X64\CheckPrimeAppUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2017\X64
if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2019\X64\CheckPrimeAppUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2019\X64

:EndIfTag

if %EXE_BUILD_DIR%=="" (
goto ErrorLabel1
)

pushd %EXE_BUILD_DIR%

::dir CheckPrimeAppUnitTestHost
echo Before call  CheckPrimeAppUnitTestHost.exe
call  CheckPrimeAppUnitTestHost.exe
popd

if %ERRORLEVEL% NEQ 0 goto ErrorLabel2
goto Exit

:ErrorLabel0
echo Error - Set TARGET_ARCH = X64 in Conf/Target.txt or use -a X64 with Build command
echo 

:ErrorLabel1
echo Error - no CheckPrimeAppUnitTestHost.exe found 

:ErrorLabel2
set exitCode=1
goto Exit

:Usage
echo.
echo ***************************************************************************
echo Usage: RunHostTestCheckPrimeApp [TOOL_CHAIN_TAG]
echo.
echo   Where TOOL_CHAIN_TAG is VS2015x86 or VS2017

:Exit
echo exiting RunHostTestCheckPrimeApp




