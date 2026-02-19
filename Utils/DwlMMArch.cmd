@echo off

if not exist %~dp0\mmarch.exe (
	
	if not exist %~dp0\mmarch.7z (
		echo Downloading the missing 'mmarch.exe' tool...
		bitsadmin.exe /transfer MMArchDownload /download /priority FOREGROUND "https://github.com/might-and-magic/mmarch/releases/download/v3.2/mmarch.7z" %~dp0\mmarch.7z || exit /B 1
	)

	pushd %~dp0

	echo Extracting the missing 'mmarch.exe' tool...
	if exist "%ProgramFiles%\7-Zip\7z.exe" (
		"%ProgramFiles%\7-Zip\7z.exe" x mmarch.7z mmarch.exe -bso0
	) else (
		tar -xf mmarch.7z mmarch.exe
	)

	popd
)
