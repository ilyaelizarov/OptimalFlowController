@echo off
:: script is supposed to be executed in /build directory

if exist ..\bin\release_x64\ShowParameters.dll goto DLL_EXISTS
echo "ERROR: File ShowParameters.dll expected in directory ..\bin\release_x64\ShowParameters.dll, but does not exist.
exit /b 1
:DLL_EXISTS

:: remove target directory if it exists
if not exist ShowParameters goto DIRECTORY_CLEAN
echo Removing existing directory 'ShowParameters'
rd /S /Q "ShowParameters"
:DIRECTORY_CLEAN

:: remove target FMU if it exists
if not exist ShowParameters.fmu goto FMU_REMOVED
echo Removing existing FMU file 'ShowParameters.fmu'
del /F /S /Q "ShowParameters.fmu"
:FMU_REMOVED

::create subdir and change into it
mkdir ShowParameters

cd ShowParameters

:: create binary dir for Windows
mkdir binaries\win64

:: copy shared library, we expect it to be already renamed correctly
xcopy ..\..\bin\release_x64\ShowParameters.dll binaries\win64\
xcopy ..\..\data\modelDescription.xml .
echo Created FMU directory structure

::change working directory back to original dir
cd ..

::create zip archive
echo Creating archive 'ShowParameters.zip'
cd ShowParameters
"C:\Program Files\7-Zip\7z.exe" a ../ShowParameters.zip .
cd ..

echo Renaming archive to 'ShowParameters.fmu'
rename ShowParameters.zip ShowParameters.fmu

:: all ok
exit /b 0
