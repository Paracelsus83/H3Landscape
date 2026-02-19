# Finding / downloading the MMArch tool for creating .lod archives

cmake_minimum_required(VERSION 3.20)

find_program(MMARCH "mmarch.exe" PATHS "Utils" DOC "Path to the MMarch tool")

if(NOT MMARCH)
	file(DOWNLOAD
		"https://github.com/might-and-magic/mmarch/releases/download/v3.2/mmarch.7z"
		"Utils/mmarch.7z"
		EXPECTED_HASH MD5=f9b805b1aecc6f44e6325b8d6b35b3bc
		STATUS status
	)
	list(GET status 0 status_code)
	if(status_code EQUAL 0)
		execute_process(COMMAND
			tar -xf mmarch.7z mmarch.exe
			WORKING_DIRECTORY Utils
		)
		if(EXISTS "Utils/mmarch.exe")
			set(MMARCH "Utils/mmarch.exe" CACHE FILEPATH "Path to the MMarch tool" FORCE)
			message("-- MMArch tool downloaded: ${MMARCH}")
		endif()
	else()
		message("The MMarch tool could not be downloaded.")
	endif()
endif()
