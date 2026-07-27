cmake_minimum_required(VERSION 3.20)

# ===== list of source files ===== #

set(SOURCE_FILES
	main.cpp
	Win32EXE_Lib/exe_file.hpp
	Win32EXE_Lib/exe_file.cpp
	Win32EXE_Lib/min_win.hpp
	Win32EXE_Lib/rsrc_parser.cpp
)

set(RESOURCE_FILES
	resource.h
	Patcher.rc
)

# ===== building the H3L_HotA_Patcher.exe ===== #

add_executable(patcher WIN32 ${SOURCE_FILES} ${RESOURCE_FILES})
set_target_properties(patcher PROPERTIES OUTPUT_NAME "H3L_HotA_Patcher")

if(CMAKE_CXX_CPPCHECK)
	set_property(TARGET patcher APPEND PROPERTY CXX_CPPCHECK
		"--suppress=uninitMemberVar:AT4RE_Lib/pefile.hpp"
	)
endif()

# ===== installation of the H3L_HotA_Patcher.exe ===== #

if(HD3_DATA_PATH)
	cmake_path(GET HD3_DATA_PATH PARENT_PATH H3_ROOT_PATH)
	install(TARGETS patcher RUNTIME DESTINATION ${H3_ROOT_PATH})
endif()
