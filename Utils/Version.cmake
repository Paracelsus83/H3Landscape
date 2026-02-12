# Determining the project version based on data from the GIT repository

cmake_minimum_required(VERSION 3.20)

execute_process(
	COMMAND git describe --tags --abbrev=4 --always --dirty
	OUTPUT_VARIABLE VERSION_FROM_GIT
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(VERSION_FROM_GIT)
	message("-- H3Landscape version: ${VERSION_FROM_GIT}")
	
	if(VERSION_FROM_GIT MATCHES "^v([0-9]+\.[0-9]+\.[0-9]+)(-([0-9])+-)?")
		if(CMAKE_MATCH_2)
			set(H3L_COMMIT_NUM ${CMAKE_MATCH_3})
		else()
			set(H3L_COMMIT_NUM 0)
			add_compile_definitions($<$<CONFIG:Release>:H3L_RELEASE>)
		endif()
		set(H3L_PROJECT_VERSION "${CMAKE_MATCH_1}.${H3L_COMMIT_NUM}")
		string(SUBSTRING "${VERSION_FROM_GIT}" 1 -1 H3L_VERSION)
	else()
		set(H3L_VERSION "${VERSION_FROM_GIT}")
	endif()
else()
	string(TIMESTAMP H3L_VERSION "%Y-%m-%d")
endif()

if(NOT H3L_PROJECT_VERSION)
	set(H3L_PROJECT_VERSION 0.0.0.0)
endif()

string(REPLACE "." "," H3L_PRODUCT_VERSION ${H3L_PROJECT_VERSION})

add_compile_definitions($<$<COMPILE_LANGUAGE:RC>:H3LANDSCAPE_VERSION=${H3L_VERSION}>)
add_compile_definitions($<$<COMPILE_LANGUAGE:RC>:H3L_PROD_VERSION=${H3L_PRODUCT_VERSION}>)
