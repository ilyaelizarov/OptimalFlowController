@echo off

:: setup VC environment variables
if not defined DevFMICodeGenerator (
set VCVARSALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
call %VCVARSALL_PATH%
)

set DevFMICodeGenerator=1

:: FMU-specific variables - set by code generator
set FMU_SHARED_LIB_NAME=ShowParameters.dll

set CMAKELISTSDIR=%CD%\..\projects\cmake

:: create and change into build subdir
mkdir bb_VC_x64
pushd bb_VC_x64

:: configure makefiles and build
cmake -G "NMake Makefiles" %CMAKELISTSDIR% -DCMAKE_BUILD_TYPE:String="Release"
nmake
if ERRORLEVEL 1 GOTO fail

popd

:: copy executable to bin/release dir
xcopy /Y .\bb_VC_x64\%FMU_SHARED_LIB_NAME% ..\bin\release_x64

exit /b 0

:fail
echo ** Build Failed **
exit /b 1
