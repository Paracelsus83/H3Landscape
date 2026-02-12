# Using the compilation tools provided with Ms Visual Studio

cmake_minimum_required(VERSION 3.20)

cmake_path(CONVERT "$ENV{ProgramFiles\(x86\)}" TO_CMAKE_PATH_LIST PROGRAM_FILES_X86)

if(NOT MSVC_BASE_PATH)
	set(VSWHERE_EXE_PATH "${PROGRAM_FILES_X86}/Microsoft Visual Studio/Installer/vswhere.exe")
	if (EXISTS "${VSWHERE_EXE_PATH}")
		execute_process(COMMAND "${VSWHERE_EXE_PATH}" -latest -property installationPath
			OUTPUT_VARIABLE MS_VS_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)

		cmake_path(CONVERT "${MS_VS_PATH}\\VC" TO_CMAKE_PATH_LIST _MSVC_BASE_PATH)
		if(EXISTS "${_MSVC_BASE_PATH}")
			message("-- Ms Visual Studio (C++) found: ${_MSVC_BASE_PATH}")
			set(MSVC_BASE_PATH "${_MSVC_BASE_PATH}" CACHE PATH "Base path to Ms Visual Studio tools for C++")
		else()
			message(WARNING "Microsoft Visual Studio tools for C++ were not found.")
		endif()
	endif()
endif()

if(MSVC_BASE_PATH)
	if(NOT CMAKE_MAKE_PROGRAM)
		file(STRINGS "${MSVC_BASE_PATH}/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt" VCTOOLSET_VERSION)
		find_program(
			CMAKE_MAKE_PROGRAM
			"nmake.exe"
			PATHS "${MSVC_BASE_PATH}/Tools/MSVC/${VCTOOLSET_VERSION}/bin/Hostx64/x86"
			REQUIRED
		)
	endif()

	set(LLVM_BIN_PATH "${MSVC_BASE_PATH}/Tools/Llvm/bin")
	if(EXISTS "${LLVM_BIN_PATH}")
		if(NOT EXISTS "${CMAKE_CXX_COMPILER}")
			if(NOT CMAKE_CXX_COMPILER)
				# Use the Clang-CL compiler from Ms Visual Studio - if installed
				set(CLANG_VER "clang-cl")
			elseif(CMAKE_CXX_COMPILER STREQUAL "clang++")
				# Use the Clang++ compiler from Ms Visual Studio - if installed
				set(CLANG_VER "clang++")
			endif()
			if(CLANG_VER)
				set(CLANG_PATH "${LLVM_BIN_PATH}/${CLANG_VER}.exe")
				if(EXISTS "${CLANG_PATH}")
					set(CMAKE_CXX_COMPILER "${CLANG_PATH}")
				else()
					find_program(CMAKE_CXX_COMPILER "clang++")
				endif()
			endif()
		endif()
		if(NOT CMAKE_CXX_CLANG_TIDY)
			set(CLANG_TIDY_PATH "${LLVM_BIN_PATH}/clang-tidy.exe")
			if(EXISTS "${CLANG_TIDY_PATH}")
				set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
				set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_PATH};-checks='*';-p;.")
			endif()
		endif()
	endif() # LLVM
endif()

# If compiler is not G++ (GNU)
if(NOT CMAKE_CXX_COMPILER MATCHES "[^A-Za-z][Gg][+][+](\.exe)?$")
	# Use Microsoft Windows SDK v7.1A - if installed
	set(MS_SDK_PATH "${PROGRAM_FILES_X86}/Microsoft SDKs/Windows/v7.1A")
	if (EXISTS "${MS_SDK_PATH}")
		message("-- Windows SDK v7.1A found: ${MS_SDK_PATH}")
		set(CMAKE_RC_COMPILER "${MS_SDK_PATH}/Bin/RC.exe")
		include_directories(SYSTEM "${MS_SDK_PATH}/Include")
		link_directories(BEFORE "${MS_SDK_PATH}/Lib")
	else()
		message(WARNING "Microsoft Windows SDK v7.1A not found; It is recommended to install it.")
	endif()
endif()
