# Static analysis of C++ code using CppCheck

cmake_minimum_required(VERSION 3.20)

set(CPPCHECK_LEVEL "reduced" CACHE STRING "Configure how much valueflow analysis you want")

if(NOT CMAKE_CXX_CPPCHECK)
	cmake_path(CONVERT "$ENV{ProgramFiles}" TO_CMAKE_PATH_LIST PROGRAM_FILES)
	find_program(CMAKE_CXX_CPPCHECK "cppcheck" PATHS "${PROGRAM_FILES}/Cppcheck")
endif()

if(CMAKE_CXX_CPPCHECK)
	list(APPEND CMAKE_CXX_CPPCHECK
		--platform=win32A
		--library=std --library=windows
		--library=${CMAKE_CURRENT_LIST_DIR}/Win32MSVC.cccfg
		--std=c++${CMAKE_CXX_STANDARD}
		--enable=warning,performance,style
		--check-level=${CPPCHECK_LEVEL}
	)
endif()
