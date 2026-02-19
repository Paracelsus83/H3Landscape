cmake_minimum_required(VERSION 3.20)

if(CMAKE_GENERATOR STREQUAL "NMake Makefiles")
	include(Utils/NMake.cmake)
endif()
if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set(CMAKE_GENERATOR_PLATFORM Win32)
	set(CMAKE_GENERATOR_TOOLSET v141_xp)
	set(CMAKE_VS_GLOBALS "XPDeprecationWarning=false")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
add_compile_definitions(_SBCS _WIN32_WINNT=_WIN32_WINNT_WINXP)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/Release")

include(Utils/CppCheck.cmake)
include(Utils/MMarch.cmake)
include(Utils/Version.cmake)


project(H3LandscapeCMake
	VERSION ${H3L_PROJECT_VERSION}
	LANGUAGES CXX
	HOMEPAGE_URL "https://github.com/Paracelsus83/H3Landscape/"
)

# Enable most build warnings
if(MSVC)
	add_compile_options(/W4 /WX)
elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
	add_compile_options(-m32 -Wall -Wextra)
endif()

# Find Heroes III installation path
find_file(HD3_DATA_PATH "_HD3_Data" PATHS
	"[HKLM/SOFTWARE/WOW6432Node/New World Computing/Heroes of Might and Magic® III/1.0;AppPath]"
	"[HKCU/SOFTWARE/Classes/VirtualStore/MACHINE/SOFTWARE/WOW6432Node/New World Computing/Heroes of Might and Magic® III/1.0;AppPath]"
	"[HKLM/SOFTWARE/WOW6432Node/Microsoft/Windows/CurrentVersion/Uninstall/HotA + HD_is1;Inno Setup: App Path]"
)

add_subdirectory(Landscape Landscape.build)

add_subdirectory(Patcher Patcher.build)
