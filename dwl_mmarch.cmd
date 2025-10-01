@echo off
setlocal EnableDelayedExpansion

if not exist mmarch.exe (
	set DWL_DIR=%~dp0\LodBuild
	set DWL_FILE="!DWL_DIR!\mmarch.7z"
	
	if not exist !DWL_FILE! (
		echo *** Downloading the missing 'mmarch.exe' tool...
		if not exist "!DWL_DIR!" mkdir "!DWL_DIR!"
		bitsadmin.exe /transfer MMArchDownload /download /priority FOREGROUND "https://github.com/might-and-magic/mmarch/releases/download/v3.2/mmarch.7z" !DWL_FILE!
	)

	echo *** Extracting the missing 'mmarch.exe' tool...
	if exist "%ProgramFiles%\7-Zip\7z.exe" (
		"%ProgramFiles%\7-Zip\7z.exe" x !DWL_FILE! mmarch.exe "-o%~dp0\Landscape" -bso0
	) else (
		tar -C "%~dp0\Landscape" -xf !DWL_FILE! mmarch.exe
	)
)
