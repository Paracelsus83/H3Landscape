cmake_path(CONVERT "$ENV{ProgramFiles\(x86\)}" TO_CMAKE_PATH_LIST PROGRAM_FILES_X86)

set(VSWHERE_EXE_PATH "${PROGRAM_FILES_X86}/Microsoft Visual Studio/Installer/vswhere.exe")
if (EXISTS "${VSWHERE_EXE_PATH}")
	execute_process(COMMAND "${VSWHERE_EXE_PATH}" -latest -property installationPath
		OUTPUT_VARIABLE MSVC_BASE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
		
	cmake_path(CONVERT "${MSVC_BASE_PATH}" TO_CMAKE_PATH_LIST MSVC_BASE_PATH)
	if(EXISTS "${MSVC_BASE_PATH}")
		file(STRINGS "${MSVC_BASE_PATH}/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt" VCTOOLSET_VERSION)
		set(MSVC_NMAKE_PATH "${MSVC_BASE_PATH}/VC/Tools/MSVC/${VCTOOLSET_VERSION}/bin/Hostx64/x86/nmake.exe")
		if(EXISTS "${MSVC_NMAKE_PATH}")
			message("-- NMake found: ${MSVC_NMAKE_PATH}")
			set(CMAKE_MAKE_PROGRAM "${MSVC_NMAKE_PATH}")
		endif()

		# Use the Clang-CL compiler from Ms Visual Studio - if installed
		set(CLANG_CL_PATH "${MSVC_BASE_PATH}/VC/Tools/Llvm/bin/clang-cl.exe")
		if(EXISTS "${CLANG_CL_PATH}")
			set(CMAKE_CXX_COMPILER "${CLANG_CL_PATH}")
		endif()

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
endif()
