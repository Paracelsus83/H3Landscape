cmake_minimum_required(VERSION 3.20)

if(WIN32 AND CMAKE_GENERATOR STREQUAL "NMake Makefiles")
	include(NMake.cmake)
endif()
if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set(CMAKE_GENERATOR_PLATFORM Win32)
endif()


project(H3Landscape CXX)

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PROJECT_PATH)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_PATH}/${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17) # Set the C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /D_HAS_EXCEPTIONS=0 /W3 /GR- /GS- /EHs- /fp:except- /Zc:threadSafeInit-")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	add_compile_options(-m32)
endif()


# Determining the version for a DLL file
execute_process(
	COMMAND git describe --tags --abbrev=4 --always
	OUTPUT_VARIABLE VERSION_FROM_GIT
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(VERSION_FROM_GIT)
	message("-- H3Landscape version: ${VERSION_FROM_GIT}")
	string(SUBSTRING "${VERSION_FROM_GIT}" 0 1 VER_FIRST_CHAR)
	if(VER_FIRST_CHAR STREQUAL "v")
		string(SUBSTRING "${VERSION_FROM_GIT}" 1 -1 H3L_VERSION)
	else()
		set(H3L_VERSION "${VERSION_FROM_GIT}")
	endif()
else()
	set(H3L_VERSION "unknown")
endif()


# Use NH3API library headers
include_directories("../NH3API")

set(SOURCE_FILES
	dllmain.cpp
	game_mod/battlefield.cpp
	game_mod/img_loader.cpp
)

set(HEADER_FILES
	game_mod/asm_helper.h
	game_mod/battlefield.hpp
	game_mod/hota_terrain.hpp
	game_mod/img_loader.hpp
)

set(RESOURCE_FILES
	resource.h
	Landscape.rc
)

set(IMAGE_FILES
	img/CmBkCFUg.bmp
	img/CmBkCstUg.bmp
	img/CmBkDesUg.bmp
	img/CmBkDkUg.bmp
	img/CmBkDrUg.bmp
	img/CmBkDunUg.bmp
	img/CmBkEFUg.bmp
	img/CmBkFFUg.bmp
	img/CmBkFGUg.bmp
	img/CmBkFGrUg.bmp
	img/CmBkGrUg.bmp
	img/CmBkHGUg.bmp
	img/CmBkIceUg.bmp
	img/CmBkLPUg.bmp
	img/CmBkLkUg.bmp
	img/CmBkLvUg.bmp
	img/CmBkMCDk.bmp
	img/CmBkMCUg.bmp
	img/CmBkMgUg.bmp
	img/CmBkRedMt.bmp
	img/CmBkRghUg.bmp
	img/CmBkRkUg.bmp
	img/CmBkSnUg.bmp
	img/CmBkSwUg.bmp
	img/CmBkWlUg.bmp
	img/SgBwUgBk.bmp
	img/SgCFBack.bmp
	img/SgCFMoat.bmp
	img/SgCFUgBk.bmp
	img/SgCsUgBk.bmp
	img/SgCurBack.bmp
	img/SgCvUgBk.bmp
	img/SgDnCGMlip.bmp
	img/SgDnRkMlip.bmp
	img/SgDnSfBk.bmp
	img/SgElUgBk.bmp
	img/SgFFBack.bmp
	img/SgFaMoatUg.bmp
	img/SgFaUgBk.bmp
	img/SgFrUgBk.bmp
	img/SgInUgBk.bmp
	img/SgNcUgBk.bmp
	img/SgRkBack.bmp
	img/SgRkUgBk.bmp
	img/SgRmUgBk.bmp
	img/SgStUgBk.bmp
	img/SgTwUgBk.bmp
)

# Create Landscape.dll
add_library(plugin SHARED ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES})
set_target_properties(plugin PROPERTIES OUTPUT_NAME "Landscape")
target_compile_definitions(plugin PRIVATE H3LANDSCAPE_VERSION=${H3L_VERSION})


set(LOD_FILE "${PROJECT_PATH}/LodBuild/Landscape.lod")

find_program(MMARCH_PATH "mmarch.exe" PATHS ".")
if(MMARCH_PATH)
	# Create Landscape.lod
	add_custom_command(OUTPUT ${LOD_FILE}
		COMMAND ${MMARCH_PATH} create Landscape.lod h3lod ..\\LodBuild img\\*.bmp
		DEPENDS ${IMAGE_FILES}
		WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
	)
	add_custom_target(resources ALL DEPENDS ${LOD_FILE})
else()
	message(WARNING "The mmarch.exe tool not found; Building the Landsape.lod archive is disabed.")
endif()


find_file(HD3_DATA_PATH "_HD3_Data" PATHS
	"[HKLM/SOFTWARE/WOW6432Node/New World Computing/Heroes of Might and Magic® III/1.0;AppPath]"
	"[HKCU/SOFTWARE/Classes/VirtualStore/MACHINE/SOFTWARE/WOW6432Node/New World Computing/Heroes of Might and Magic® III/1.0;AppPath]"
	"[HKLM/SOFTWARE/WOW6432Node/Microsoft/Windows/CurrentVersion/Uninstall/HotA + HD_is1;Inno Setup: App Path]"
)

if(HD3_DATA_PATH)
	set(INSTALL_DEST_PATH "${HD3_DATA_PATH}/Packs/Landscape")
	install(TARGETS plugin RUNTIME DESTINATION "${INSTALL_DEST_PATH}")
	install(FILES ${LOD_FILE} DESTINATION "${INSTALL_DEST_PATH}")
endif()
