@Echo off

::
:: These files add Ambiq Apollo4 support to IAR EWARM.
::
:: User must supply the version name of the IAR EWARM installation, which
:: can typically be found at: C:\Program Files (x86)\IAR Systems\
:: and is typically named something like: Embedded Workbench 8.5
::
:: Therefore, a typical command line will be something like:
::    install_Ambiq2IAR.bat Embedded Workbench 8.5
::

::
:: First, make sure we're administrator.
:: After the next 3 lines, the variable runState will be set as one of:
::  admin, system, or user.
::
set runState=user
whoami /groups | findstr /b /c:"Mandatory Label\High Mandatory Level" > nul && set runState=admin
whoami /groups | findstr /b /c:"Mandatory Label\System Mandatory Level" > nul && set runState=system

if "%runState%"=="admin" goto Admin
Echo.
Echo ERROR: This batch file must be run as Administrator!
Echo.
Echo. To start a Command Prompt as administrator:
Echo. - Press Windows+R (opens the 'Run' box)
Echo. - Type 'cmd', Ctrl+Shift+Enter, click 'Yes' to the system prompt.
Echo.
Echo.
Echo Ambiq install aborted!
Goto Exit

:Admin
set EWVer=
if "%1"=="fullpath" goto FullPath
if "%1"=="Fullpath" goto FullPath
if "%1"=="FULLpath" goto FullPath

if "%1"=="" goto GotEWVer
set EWVer=%1
if "%2"=="" goto GotEWVer
set EWVer=%1 %2
if "%3"=="" goto GotEWVer
set EWVer=%1 %2 %3
if "%4"=="" goto GotEWVer
set EWVer=%1 %2 %3 %4
if "%5"=="" goto GotEWVer
set EWVer=%1 %2 %3 %4 %5
if "%6"=="" goto GotEWVer
Echo Script error. Currently can only handle 5 arguments.
Echo Ambiq install aborted!
goto Exit

:FullPath
:: Full path specified in parm 2
:: Parm 2 must be fully specified and quoted.
if exist %2 goto fullpathGood
Echo.
Echo. ERROR: The fullpath specified was not valid.
Echo.        Be sure to put quotes around the entire path.
Echo.        Path must include drive name and the entire path all the way out to 'IAR Systems\Embedded Workbench 8.x'
Echo.
Echo ERROR: IAR EWARM is not installed.
Echo Ambiq IAR install aborted!
goto Exit

:fullpathGood
set fullpath=%2
goto ChkIAR

:GotEWVer
set fullpath="%ProgramFiles(x86)%\IAR Systems\%EWVer%"
if not "%EWVer%"=="" goto ChkIAR
goto Usage


:ChkIAR
if exist %fullpath% goto IAR_installed
Echo.
Echo. ERROR: Specified IAR install directory is incorrect.
Echo.
Echo ERROR: IAR EWARM is not installed.
Echo Ambiq IAR install aborted!
goto Exit


:IAR_installed
if exist %fullpath%\arm goto VersValid
goto Usage


:VersValid
Echo.
REM Echo Valid target: %ProgramFiles(x86)%\IAR Systems\%EWver%\arm\
Echo Valid target: %fullpath%\arm\
Echo.

::
:: Copy AmbiqMicro files to IAR installation.
::
Echo Copying files to target IAR installation directory:
REM xcopy /v /e /y *.* "%ProgramFiles(x86)%\IAR Systems\%EWver%\arm\*.*"
xcopy /v /e /y *.* %fullpath%\arm\*.*

:: Delete this batch file from the target.
REM if exist "%ProgramFiles(x86)%\IAR Systems\%EWver%\arm\install_Ambiq2IAR.bat" del "%ProgramFiles(x86)%\IAR Systems\%EWver\arm\install_Ambiq2IAR.bat"
if exist %fullpath%\install_Ambiq2IAR.bat del %fullpath%\arm\install_Ambiq2IAR.bat

Echo.
Echo Installation SUCCESS!!
Goto Exit


:Usage
Echo.
Echo. ERROR:
Echo.   Must provide the name of the EWARM installation directory, which
Echo.   can typically be found at:  C:\Program Files (x86)\IAR Systems\
Echo.   It is typically named something like: "Embedded Workbench 8.5"
Echo.
Echo.   Echo.   Installation method a.
Echo.   In this case, [Embedded Workbench 8.5] would be the argument
Echo.   to be provided for this script (no quote marks, no brackets).
Echo.   e.g.
Echo.
Echo.    install_Ambiq2IAR.bat Embedded Workbench 8.5
Echo.
Echo.   Installation method b.
Echo.   If the IAR EWARM installation is at a non-typical installation location
Echo.   such as a drive other than C:, then a full path method is required.
Echo.   For this method, specify the full path as in the following example:
Echo.
Echo.    install_Ambiq2IAR.bat fullpath "D:\Program Files (x86)\IAR Systems\Embedded Workbench 8.5"
Echo.
Echo Ambiq install aborted!
goto Exit


:Exit
Echo.
Echo Press any key to exit.
pause
set EWver=
set runState=
set IARpath=
set fullpath=
