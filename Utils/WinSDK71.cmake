# Use Microsoft Windows SDK v7.1 - if installed

cmake_minimum_required(VERSION 3.20)

set(MS_SDK_BASE_PATH "${PROGRAM_FILES_X86}/Microsoft SDKs/Windows")
find_path(MS_SDK_PATH
	"Include"
	PATHS 
		"${MS_SDK_BASE_PATH}/v7.1A"
		"${MS_SDK_BASE_PATH}/v7.1"
	NO_DEFAULT_PATH
)
if (MS_SDK_PATH)
	message("-- Windows SDK v7.1 found: ${MS_SDK_PATH}")
	if(NOT CMAKE_RC_COMPILER)
		find_program(CMAKE_RC_COMPILER
			"RC"
			PATHS "${MS_SDK_PATH}/Bin"
			REQUIRED
		)
	endif()
	include_directories(SYSTEM "${MS_SDK_PATH}/Include")
	link_directories(BEFORE "${MS_SDK_PATH}/Lib")
else()
	message(WARNING "Microsoft Windows SDK v7.1 not found; It is recommended to install it.")
endif()
