@echo off
setlocal EnableDelayedExpansion EnableExtensions
if "%WORKSPACE%"=="" (
set WORKSPACE=%CD%
)
set exitCode=0
set errorcode=0
set EXE_BUILD_DIR=""
if /i "%~1"=="/?" goto Usage

if "%~1"=="" goto findPath
Set TAG=%~1
if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_%TAG%\X64\CheckNumberPlusOneUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_%TAG%\X64
goto EndIfTag

:findPath

if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2013x86\X64\CheckNumberPlusOneUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2013x86\X64
if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2015x86\X64\CheckNumberPlusOneUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2015x86\X64
if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2017\X64\CheckNumberPlusOneUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2017\X64
if exist %WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2019\X64\CheckNumberPlusOneUnitTestHost.exe Set EXE_BUILD_DIR=%WORKSPACE%\Build\UnitTestFrameworkPkg\HostTest\NOOPT_VS2019\X64

:EndIfTag

if %EXE_BUILD_DIR%=="" (
goto ErrorLabel1
)

pushd %EXE_BUILD_DIR%

::dir CheckNumberPlusOneUnitTestHost
echo Before call  CheckNumberPlusOneUnitTestHost.exe
set CMOCKA_MESSAGE_OUTPUT=xml
set CMOCKA_XML_FILE=test.xml
if exist test.xml del test.xml
call  CheckNumberPlusOneUnitTestHost.exe >nul
findstr "<failure>" test.xml >nul
if %ERRORLEVEL% == 0 (
echo FAILURE in unit test occured
set errorcode=1
goto testfail1
) 
echo Test Passed


:testfail1
set CMOCKA_MESSAGE_OUTPUT=
popd


if %errorcode% ==1 goto ErrorLabel2
goto Exit

:ErrorLabel0
echo Error - Set TARGET_ARCH = X64 in Conf/Target.txt or use -a X64 with Build command
echo 

:ErrorLabel1
echo Error - no CheckNumberPlusOneUnitTestHost.exe found 

:ErrorLabel2
REM echo Error with errorcode = %errorcode%
echo exiting RunHostTestCheckNumberApp with errorlevel %errorcode%

  EXIT /B 1
GOTO :EOF

:Usage
echo.
echo ***************************************************************************
echo Usage: RunHostTestCheckNumberApp [TOOL_CHAIN_TAG]
echo.
echo   Where TOOL_CHAIN_TAG is VS2015x86 or VS2017

:Exit
echo exiting RunHostTestCheckNumberApp
EXIT /B 0




