@rem Copy and pack all dependencies

set QTDIR=%~2
set PROJECTDIR=%~3
set RELATIVEPTH=msvc\x64\%1%\
if %1%=="Release" (
	set CONFIGBUILD=release
) else (
	set CONFIGBUILD=debug
)

REM QT
ECHO %QTDIR%\windeployqt %PROJECTDIR%%RELATIVEPTH%Rheed3.exe -%1%
%QTDIR%\windeployqt %PROJECTDIR%%RELATIVEPTH%Rheed3.exe -%CONFIGBUILD%

REM document_DLL
set SOURCEDLLPTH=%PROJECTDIR%PMS_document_SDK\Out\x64\%1%\PMS_document_SDK.dll
ECHO copy %SOURCEDLLPTH% %PROJECTDIR%%RELATIVEPTH%
if exist %SOURCEDLLPTH% (
	copy %SOURCEDLLPTH% %PROJECTDIR%%RELATIVEPTH%
)

REM cameras
set PLGNTARGET=%PROJECTDIR%%RELATIVEPTH%plugins_cameras\

if not exist %PLGNTARGET% (
	mkdir %PLGNTARGET%
)

set SOURCEDLLPTH=%PROJECTDIR%PMS_virtual_camera\Out\x64\%1%\plgn_virtual_camera.dll
ECHO copy %SOURCEDLLPTH% %PLGNTARGET%
if exist %SOURCEDLLPTH% (
	copy %SOURCEDLLPTH% %PLGNTARGET%
)


rem exit
