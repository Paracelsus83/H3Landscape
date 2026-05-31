cmake_minimum_required(VERSION 3.20)

if(MSVC) # remove the default compilation option /EHsc
	string(REPLACE /EHsc "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# ===== list of source files ===== #

set(SOURCE_FILES
	dllmain.cpp
	game_mod/adv_map.cpp
	game_mod/adv_mt.cpp
	game_mod/battlefield.cpp
	game_mod/img_loader.cpp
	game_mod/obstacles.cpp
)

set(HEADER_FILES
	game_mod/adv_map.hpp
	game_mod/adv_mt.hpp
	game_mod/asm_helper.h
	game_mod/asm_patch.hpp
	game_mod/battlefield.hpp
	game_mod/hota_terrain.hpp
	game_mod/img_loader.hpp
	game_mod/mode.hpp
	game_mod/types.hpp
)

set(RESOURCE_FILES
	resource.h
	Landscape.rc
)

set(IMAGE_FILES
	img/CmBkCFUg.bmp
	img/CmBkCstUg.bmp
	img/CmBkDeU.bmp
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
	img/CmBkMCBt.bmp
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

set(SPRITE_FILES
	sprites/adv_obj/AVMwwhRg.def
	sprites/adv_obj/AVStmpl1.def
	sprites/adv_obj/AVSuniv1.def
	sprites/adv_obj/AVXhilSd.def
	sprites/adv_obj/AVXhilU0.def
	sprites/adv_obj/AVXruiSn.def
	sprites/adv_obj/AVXstbl1.def
	sprites/adv_obj/AVXtrek3.def
	sprites/adv_obj/AVXtrkLv.def
	sprites/adv_obj/GoRlava.def
	sprites/adv_obj/GoRsnow.def
	sprites/adv_obj/HAPCavSn.def
	sprites/adv_obj/Sanct_Sd.def
	sprites/adv_obj/UtopiaSn.def
	sprites/adv_towns/AVCCasCS.def
	sprites/adv_towns/AVCCasFS.def
	sprites/adv_towns/AVCCasVS.def
	sprites/adv_towns/AVCCasXS.def
	sprites/adv_towns/AVCCasZS.def
	sprites/adv_towns/AVCInfCS.def
	sprites/adv_towns/AVCInfFS.def
	sprites/adv_towns/AVCInfVS.def
	sprites/adv_towns/AVCInfXS.def
	sprites/adv_towns/AVCInfZS.def
	sprites/adv_towns/AVCNecCS.def
	sprites/adv_towns/AVCNecFS.def
	sprites/adv_towns/AVCNecVS.def
	sprites/adv_towns/AVCNecXS.def
	sprites/adv_towns/AVCNecZS.def
	sprites/adv_towns/AVCStrCS.def
	sprites/adv_towns/AVCStrFS.def
	sprites/adv_towns/AVCStrVS.def
	sprites/adv_towns/AVCStrXS.def
	sprites/adv_towns/AVCStrZS.def
	sprites/adv_towns/AVCTowCG.def
	sprites/adv_towns/AVCTowFG.def
	sprites/adv_towns/AVCTowVG.def
	sprites/adv_towns/AVCTowXG.def
	sprites/adv_towns/AVCTowZG.def
	sprites/obstacles/ObBDt1Ug.def
	sprites/obstacles/ObDtS3Ug.def
	sprites/obstacles/ObDtS4Ug.def
	sprites/obstacles/ObGMs1Ug.def
	sprites/obstacles/ObGMs2Ug.def
	sprites/obstacles/OBSnCrUg.def
	sprites/obstacles/ObSnRk3U.def
	sprites/obstacles/ObSnRk5U.def
	sprites/obstacles/ObSnRk6U.def
	sprites/obstacles/ObSwMsUg.def
)

# ===== building the Landscape.dll plug-in ===== #

add_library(plugin SHARED ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES})
set_target_properties(plugin PROPERTIES OUTPUT_NAME "Landscape")

target_compile_definitions(plugin PRIVATE _HAS_EXCEPTIONS=0)
target_include_directories(plugin PRIVATE "../NH3API") # Use NH3API library headers

if(MSVC)
	target_compile_options(plugin PRIVATE /GR- /GS- /EHs- /fp:except- /Zc:threadSafeInit-)
elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
	target_compile_options(plugin PRIVATE -fno-exceptions -fno-threadsafe-statics)
endif()

if(CMAKE_CXX_CPPCHECK)
	set_property(TARGET plugin APPEND PROPERTY CXX_CPPCHECK "--suppress=*:nh3api/*")
	if(MSVC)
		set_property(TARGET plugin APPEND PROPERTY CXX_CPPCHECK
			"-D_HAS_EXCEPTIONS=0"
			"-I../NH3API"
		)
	endif()
endif()

# ===== building the Landscape.lod archive ===== #

set(LOD_FILE "../LodBuild/Landscape.lod")

if(MMARCH)
	add_custom_command(OUTPUT ${LOD_FILE}
		COMMAND ${MMARCH} create Landscape.lod h3lod ..\\LodBuild img\\*.bmp sprites\\adv_obj\\*.def sprites\\adv_towns\\*.def sprites\\obstacles\\*.def
		DEPENDS ${IMAGE_FILES} ${SPRITE_FILES}
		WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
	)
	add_custom_target(resources ALL DEPENDS ${LOD_FILE})
else()
	message(WARNING "The mmarch.exe tool not found; Building the Landscape.lod is disabled.")
endif()

# ===== installation of Landscape.* files ===== #

if(HD3_DATA_PATH)
	set(INSTALL_DEST_PATH "${HD3_DATA_PATH}/Packs/Landscape")
	install(TARGETS plugin RUNTIME DESTINATION ${INSTALL_DEST_PATH})
	if(MMARCH)
		install(FILES ${LOD_FILE} DESTINATION ${INSTALL_DEST_PATH})
	endif()
endif()
