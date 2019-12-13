REM used to compile .qrc resource files in the same folder to corresponding .rcc files 

set QTDIR=%~1
set PROJECTDIR=%~2res

for %%i in (%PROJECTDIR%\*.qrc) do (
echo %QTDIR%\rcc -binary %%i -o %PROJECTDIR%\%%~ni.rcc
%QTDIR%\rcc -binary %%i -o %PROJECTDIR%\%%~ni.rcc
)

