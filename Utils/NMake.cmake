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
		if(CMAKE_GENERATOR STREQUAL "NMake Makefiles")
			file(STRINGS "${MSVC_BASE_PATH}/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt" VCTOOLSET_VERSION)
			find_program(
				CMAKE_MAKE_PROGRAM
				"nmake.exe"
				PATHS "${MSVC_BASE_PATH}/Tools/MSVC/${VCTOOLSET_VERSION}/bin/Hostx64/x86"
				REQUIRED
			)
		elseif(CMAKE_GENERATOR STREQUAL "Ninja")
			find_program(
				CMAKE_MAKE_PROGRAM
				"ninja"
				PATHS "${MSVC_BASE_PATH}/../Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja"
				REQUIRED
			)
		endif()
	endif()

	find_path(LLVM_BIN_PATH
		"."
		PATHS 
			"${MSVC_BASE_PATH}/Tools/Llvm/x64/bin"
			"${MSVC_BASE_PATH}/Tools/Llvm/bin"
		NO_DEFAULT_PATH
	)
	if(LLVM_BIN_PATH)
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
				set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_PATH};--config-file=${CMAKE_CURRENT_LIST_DIR}/ClangTidy.conf;-p;.")
			endif()
		endif()
	else() # LLVM_BIN_PATH not exist
		message(WARNING "LLVM Clang compiler not found; It is recommended to install it.")
	endif()
endif()
