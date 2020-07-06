@echo off
setlocal EnableDelayedExpansion EnableExtensions

call  RunHostTestXML.bat
if %ERRORLEVEL% == 0 echo PASSED unit test calling RunHostTestXML.bat
echo exiting testtest





