::usage
::1 config 
::2 pathFromTestDirectoryAndProjectName 
::3 projectNameForLogFile 
::4 visualStudioVersion (2008, 2010, 2013) 
::5 arch(x86, amd64)
::6 failureIsSuccess

@echo off
setlocal

set passIfStringFound=", 0 failed,"

if "%6" == "fail" (
set passIfStringFound=", 1 failed,"
)

set projectExtension=vcproj
set visualStudioVersion=9
set programFiles="Program Files"

if (%4) == (2010) (
set projectExtension=vcxproj
set visualStudioVersion=10
)
if (%4) == (2013) (
set projectExtension=vcxproj
set visualStudioVersion=12
)
if exist C:\Program Files (x86) (
set programFiles=Program Files (x86)
)


set build_log="\\?\%cd%\build_logs\%3_vs%4_%5_%1.log"


call "C:\%programFiles%\Microsoft Visual Studio %visualStudioVersion%.0\VC\vcvarsall.bat" %5 > NUL

::if paramater 6 is not present then say what we are doing
if "%6" == "" (
	@echo building %2 %1
)
::devenv  "%cd%\oolua.sln" /build %1 /project "%cd%\unit_tests\%2.%projectExtension%" /out %build_log%
devenv  "\\?\%cd%\oolua.sln" /build %1 /project "unit_tests\%2.%projectExtension%" > %build_log%

@find %passIfStringFound% %build_log% > NUL
if %ERRORLEVEL% EQU 0 (
	::if paramater 6 is not present then report a success
	if "%6" == "" (
		@echo Success 
	)
	endlocal
	@exit /B 0
) else (
	@echo Test Failure %2
	@echo See the following log file for details: %build_log%
	endlocal
	set build_log=
	@exit /B 1
)
